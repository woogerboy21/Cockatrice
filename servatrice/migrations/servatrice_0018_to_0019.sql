-- Servatrice db migration from version 18 to version 19

alter table cockatrice_user_analytics add column `kick_count` smallint(6) not null after `last_login` default 0 after `last_login`;

UPDATE cockatrice_schema_version SET version=19 WHERE version=18;
