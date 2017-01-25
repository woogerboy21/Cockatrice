#include "skynet.h"
#include "settingscache.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

Skynet::Skynet()
{

}

Skynet::~Skynet()
{

}

void Skynet::JudgementDay()
{
    // create initial db connection
	initDatabase();

    // remove old stale data table content
    if (doesTableExist(prepareTableName("{prefix}_user_behavior")))
        executeSimpleQuery("drop table {prefix}_user_behavior");

    // create new table
    if (executeSimpleQuery("CREATE TABLE IF NOT EXISTS `{prefix}_user_behavior` (`id` int(7) unsigned zerofill NOT NULL auto_increment,`name` varchar(35),`kicks` int(1) DEFAULT 0,`badwords` int(1) DEFAULT 0,PRIMARY KEY(`id`)) ENGINE = MEMORY;"))
        qDebug() << "Judgement day has arrived";

    if (addUserToBehaviorTable("testme",1,2))
        qDebug() << setColumnValue("badwords", "testme", 50);

    qDebug() << getColumnIntValue("kicks", "testme");
    qDebug() << getColumnStringValue("name", "testme");
}

bool Skynet::addUserToBehaviorTable(const QString &user, const int &kickcount, const int &badwordcount)
{
	qDebug() << "Adding user " << user << " to behavior table";
    QSqlQuery *query = prepareQuery("insert into {prefix}_user_behavior (name,kicks,badwords) values (:user_name,:kick_count,:badword_count)");
    query->bindValue(":user_name", user);
    query->bindValue(":kick_count", kickcount);
    query->bindValue(":badword_count", badwordcount);

	if (query->exec())
		return true;

	return false;
}

bool Skynet::executeSimpleQuery(const QString &queryText)
{
    /*
        SIMPLE QUERIES ARE ONES THAT DO NOT REQUIRE BINDING FOR SECURITY.
        ONLY PASS NON-BIND QUERIES TO THIS FUNCTION FOR SECURITY REASONS.
    */

    qDebug() << "Executing query: " << queryText;
    QSqlQuery * query = prepareQuery(queryText);
    if (query->exec())
        return true;

    return false;
}

bool Skynet::doesTableExist(const QString &table)
{
    /* TABLE NAME PREPERATION EXPECTED TO BE PERFORMED BY CALLER BY USING prepareTableName FUNCTION */

	qDebug() << "Checking if table " << table << " exists";
	QSqlDatabase skynetDB = QSqlDatabase::database();
    if (skynetDB.tables().contains(table))
        return true;

    return false;
}

QSqlQuery * Skynet::prepareQuery(const QString &queryText)
{
	QString prefixedQueryText = queryText;
	QSqlDatabase skynetDB = QSqlDatabase::database();
	prefixedQueryText.replace("{prefix}", settingsCache->value("database/prefix").toString());
	QSqlQuery * query = new QSqlQuery(skynetDB);
	query->prepare(prefixedQueryText);
	return query;
}

QString Skynet::prepareTableName(const QString &table)
{
	QString results = table;
	results.replace("{prefix}", settingsCache->value("database/prefix").toString());
	return results;
}

void Skynet::initDatabase()
{
	qDebug() << "Adding DB connection to DB connections";
	QSqlDatabase skynetDB = QSqlDatabase::addDatabase("QMYSQL");
	skynetDB.setHostName(settingsCache->value("database/hostname").toString());
	skynetDB.setDatabaseName(settingsCache->value("database/database").toString());
	skynetDB.setUserName(settingsCache->value("database/user").toString());
	skynetDB.setPassword(settingsCache->value("database/password").toString());
	skynetDB.open();
}

bool Skynet::setColumnValue(const QString &column, const QString &user, const int &amount)
{
    QString queryText = QString("update {prefix}_user_behavior set %1 = :amount where name = :user_name").arg(column);
    QSqlQuery * query = prepareQuery(queryText);
    query->bindValue(":column", column);
    query->bindValue(":amount", amount);
    query->bindValue(":user_name", user);

    if (query->exec())
        return true;

    return false;
}

bool Skynet::increaseColumnValue(const QString &column, const QString &user, const int &amount)
{
    QString queryText = QString("update {prefix}_user_behavior set %1 = %1+:amount where name = :user_name").arg(column);
    QSqlQuery * query = prepareQuery(queryText);
    query->bindValue(":amount", amount);
    query->bindValue(":user_name", user);

    if (query->exec())
        return true;

    return false;
}

bool Skynet::decreaseColumnValue(const QString &column, const QString &user, const int &amount)
{
    QString queryText = QString("update {prefix}_user_behavior set %1 = %1-:amount where name = :user_name").arg(column);
    QSqlQuery * query = prepareQuery(queryText);
    query->bindValue(":amount", amount);
    query->bindValue(":user_name", user);

    if (query->exec())
        return true;

    return false;
}

int Skynet::getColumnIntValue(const QString &column, const QString &user)
{
    //TODO: FIGURE OUT WHY BINDING ISNT WORKING HERE
    QString queryText = QString("select %1 from {prefix}_user_behavior where name = '%2'").arg(column).arg(user);
    queryText = prepareTableName(queryText);
    QSqlQuery query(queryText);
    if (query.first())
        return query.value(column).toInt();

    return 999;
}

QString Skynet::getColumnStringValue(const QString &column, const QString &user)
{
    //TODO: FIGURE OUT WHY BINDING ISNT WORKING HERE
    QString queryText = QString("select %1 from cockatrice_user_behavior where name = '%2'").arg(column).arg(user);
    queryText = prepareTableName(queryText);
    QSqlQuery query(queryText);;
    if (query.first()) {
        return query.value(column).toString();
    }
    return "";
}