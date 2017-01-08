#ifndef SKYNET_H
#define SKYNET_H

#include <QString>

class Skynet
{
public:
    Skynet();
    ~Skynet();
    void JudgementDay();
	bool addUserToTable(const QString &userName);
	bool deleteUserFromTable(const QString &userName);
private:
    bool createDatabaseTable();
    bool deleteDatabaseTable();
    bool dbReady();
	void initDatabase();
};


#endif // SKYNET_H
