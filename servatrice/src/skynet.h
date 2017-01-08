#ifndef SKYNET_H
#define SKYNET_H


class Skynet
{
public:
    Skynet();
    ~Skynet();
    void JudgementDay();

private:
    bool createDatabaseTable();
    bool deleteDatabaseTable();
    bool dbReady();
	void initDatabase();
};


#endif // SKYNET_H
