#ifndef SKYNET_H
#define SKYNET_H

#include <QString>
#include <QSqlQuery>

class Skynet
{
public:
    Skynet();
    ~Skynet();
    void JudgementDay();

private:
	QSqlQuery * prepareQuery(const QString &queryText);
	QString prepareTableName(const QString &table);
    bool createBehaviorDatabaseTable();
    bool deleteBehaviorDatabaseTable();
    bool doesTableExist(const QString &table);
	bool addUserToBehaviorTable(const QString &user);
	void initDatabase();
};


#endif // SKYNET_H
