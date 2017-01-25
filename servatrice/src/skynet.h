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
    bool doesTableExist(const QString &table);
	bool addUserToBehaviorTable(const QString &user, const int &kickcount, const int &badwordcount);
    bool executeSimpleQuery(const QString &queryText);
    bool setColumnValue(const QString &column, const QString &user, const int &amount);
    bool increaseColumnValue(const QString &column, const QString &user, const int &amount);
    bool decreaseColumnValue(const QString &column, const QString &user, const int &amount);
    int getColumnIntValue(const QString &column, const QString &user);
    QString getColumnStringValue(const QString &column, const QString &user);
	void initDatabase();
};


#endif // SKYNET_H
