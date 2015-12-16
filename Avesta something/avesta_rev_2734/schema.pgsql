CREATE TABLE "groups" (
    "id" BIGSERIAL,
    "name" VARCHAR(255) NOT NULL,
    "flags" BIGINT NOT NULL DEFAULT 0,
    "access" INT NOT NULL,
    "maxdepotitems" INT NOT NULL,
    "maxviplist" INT NOT NULL,
    PRIMARY KEY ("id")
);

CREATE TABLE "accounts" (
    "id" BIGSERIAL,
    "password" VARCHAR(255) NOT NULL/* VARCHAR(32) NOT NULL for MD5*/,
    "email" VARCHAR(255) NOT NULL DEFAULT '',
    "blocked" SMALLINT NOT NULL DEFAULT 0,
    PRIMARY KEY ("id")
);

CREATE TABLE "players" (
    "id" BIGSERIAL,
    "name" VARCHAR(255) NOT NULL,
    "account_id" INT NOT NULL,
    "group_id" INT NOT NULL,
    "premend" BIGINT NOT NULL DEFAULT 0,
    "sex" BIGINT NOT NULL DEFAULT 0,
    "vocation" BIGINT NOT NULL DEFAULT 0,
    "experience" BIGINT NOT NULL DEFAULT 0,
    "level" BIGINT NOT NULL DEFAULT 1,
    "maglevel" BIGINT NOT NULL DEFAULT 0,
    "health" BIGINT NOT NULL DEFAULT 100,
    "healthmax" BIGINT NOT NULL DEFAULT 100,
    "mana" BIGINT NOT NULL DEFAULT 100,
    "manamax" BIGINT NOT NULL DEFAULT 100,
    "manaspent" BIGINT NOT NULL DEFAULT 0,
    "soul" BIGINT NOT NULL DEFAULT 0,
    "direction" BIGINT NOT NULL DEFAULT 0,
    "lookbody" BIGINT NOT NULL DEFAULT 10,
    "lookfeet" BIGINT NOT NULL DEFAULT 10,
    "lookhead" BIGINT NOT NULL DEFAULT 10,
    "looklegs" BIGINT NOT NULL DEFAULT 10,
    "looktype" BIGINT NOT NULL DEFAULT 136,
    "lookaddons" BIGINT NOT NULL DEFAULT 0,
    "posx" INT NOT NULL DEFAULT 0,
    "posy" INT NOT NULL DEFAULT 0,
    "posz" INT NOT NULL DEFAULT 0,
    "cap" INT NOT NULL DEFAULT 0,
    "lastlogin" BIGINT NOT NULL DEFAULT 0,
    "lastip" BIGINT NOT NULL DEFAULT 0,
    "save" SMALLINT NOT NULL DEFAULT 0,
    "conditions" BYTEA NOT NULL,
    "redskulltime" BIGINT NOT NULL DEFAULT 0,
    "redskull" SMALLINT NOT NULL DEFAULT 0,
    "guildnick" VARCHAR(255) NOT NULL DEFAULT '',
    "loss_experience" INT NOT NULL DEFAULT 10,
    "loss_mana" INT NOT NULL DEFAULT 10,
    "loss_skills" INT NOT NULL DEFAULT 10,
    "rank_id" INT NOT NULL,
    "town_id" INT NOT NULL,
    PRIMARY KEY ("id"),
    UNIQUE ("name"),
    FOREIGN KEY ("account_id") REFERENCES "accounts" ("id") ON DELETE CASCADE,
    FOREIGN KEY ("group_id") REFERENCES "groups" ("id")
);

CREATE TABLE "guilds" (
    "id" BIGSERIAL,
    "name" VARCHAR(255) NOT NULL,
    "ownerid" INT NOT NULL,
    "creationdata" INT NOT NULL,
    PRIMARY KEY ("id")
);

CREATE TABLE "guild_ranks" (
    "id" BIGSERIAL,
    "guild_id" INT NOT NULL,
    "name" VARCHAR(255) NOT NULL,
    "level" INT NOT NULL,
    PRIMARY KEY ("id"),
    FOREIGN KEY ("guild_id") REFERENCES "guilds" ("id") ON DELETE CASCADE
);

CREATE TABLE "player_viplist" (
    "player_id" INT NOT NULL,
    "vip_id" INT NOT NULL,
    FOREIGN KEY ("player_id") REFERENCES "players" ("id") ON DELETE CASCADE,
    FOREIGN KEY ("vip_id") REFERENCES "players" ("id") ON DELETE CASCADE
);

CREATE TABLE "player_spells" (
    "player_id" INT NOT NULL,
    "name" VARCHAR(255) NOT NULL,
    FOREIGN KEY ("player_id") REFERENCES "players" ("id") ON DELETE CASCADE
);

CREATE TABLE "player_storage" (
    "player_id" INT NOT NULL,
    "key" INT NOT NULL,
    "value" INT NOT NULL,
    FOREIGN KEY ("player_id") REFERENCES "players" ("id") ON DELETE CASCADE
);

CREATE TABLE "player_skills" (
    "player_id" INT NOT NULL,
    "skillid" BIGINT NOT NULL,
    "value" BIGINT NOT NULL DEFAULT 0,
    "count" BIGINT NOT NULL DEFAULT 0,
    FOREIGN KEY ("player_id") REFERENCES "players" ("id") ON DELETE CASCADE
);

CREATE TABLE "player_items" (
    "player_id" INT NOT NULL,
    "sid" INT NOT NULL,
    "pid" INT NOT NULL DEFAULT 0,
    "itemtype" INT NOT NULL,
    "count" INT NOT NULL DEFAULT 0,
    "attributes" BYTEA,
    FOREIGN KEY ("player_id") REFERENCES "players" ("id") ON DELETE CASCADE,
    UNIQUE ("player_id", "sid")
);

CREATE TABLE "houses" (
    "id" BIGSERIAL,
    "owner" INT NOT NULL,
    "paid" BIGINT NOT NULL DEFAULT 0,
    "warnings" TEXT NOT NULL,
    PRIMARY KEY ("id")
);

CREATE TABLE "house_lists" (
    "house_id" INT NOT NULL,
    "listid" INT NOT NULL,
    "list" TEXT NOT NULL,
    FOREIGN KEY ("house_id") REFERENCES "houses" ("id") ON DELETE CASCADE
);

CREATE TABLE "bans" (
    "type" INT NOT NULL,
    "ip" BIGINT NOT NULL DEFAULT 0,
    "mask" BIGINT NOT NULL DEFAULT 4294967295,
    "player" BIGINT NOT NULL DEFAULT 0,
    "account" BIGINT NOT NULL DEFAULT 0,
    "time" BIGINT NOT NULL DEFAULT 0
);

CREATE TABLE "tiles" (
    "id" BIGSERIAL,
    "x" INT NOT NULL,
    "y" INT NOT NULL,
    "z" INT NOT NULL,
    PRIMARY KEY("id")
);

CREATE TABLE "tile_items" (
    "tile_id" INT NOT NULL,
    "sid" INT NOT NULL,
    "pid" INT NOT NULL DEFAULT 0,
    "itemtype" INT NOT NULL,
    "count" INT NOT NULL DEFAULT 0,
    "attributes" BYTEA NOT NULL,
    FOREIGN KEY ("tile_id") REFERENCES "tiles" ("id") ON DELETE CASCADE
);

CREATE TABLE "player_depotitems" (
    "player_id" INT NOT NULL,
    "depot_id" INT NOT NULL DEFAULT 0,
    "sid" INT NOT NULL,
    "pid" INT NOT NULL DEFAULT 0,
    "itemtype" INT NOT NULL,
    "count" INT NOT NULL DEFAULT 0,
    "attributes" BYTEA NOT NULL,
    FOREIGN KEY ("player_id") REFERENCES "players" ("id") ON DELETE CASCADE,
    UNIQUE ("player_id", "sid")
);

CREATE TABLE "global_storage" (
    "key" INT NOT NULL,
    "value" INT NOT NULL,
    PRIMARY KEY("key")
);


CREATE FUNCTION "ondelete_accounts"()
RETURNS TRIGGER
AS $$
BEGIN
    DELETE FROM "bans" WHERE "type" = 3 AND "account" = OLD."id";

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER "ondelete_accounts"
BEFORE DELETE
ON "accounts"
FOR EACH ROW
EXECUTE PROCEDURE "ondelete_accounts"();

CREATE FUNCTION "ondelete_guilds"()
RETURNS TRIGGER
AS $$
BEGIN
    UPDATE "players" SET "guildnick" = '', "rank_id" = 0 WHERE "rank_id" IN (SELECT "id" FROM "guild_ranks" WHERE "guild_id" = OLD."id");

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER "ondelete_guilds"
BEFORE DELETE
ON "guilds"
FOR EACH ROW
EXECUTE PROCEDURE "ondelete_guilds"();

CREATE FUNCTION "ondelete_players"()
RETURNS TRIGGER
AS $$
BEGIN
    DELETE FROM "bans" WHERE "type" = 2 AND "player" = OLD."id";
    UPDATE "houses" SET "owner" = 0 WHERE "owner" = OLD."id";

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER "ondelete_players"
BEFORE DELETE
ON "players"
FOR EACH ROW
EXECUTE PROCEDURE "ondelete_players"();

CREATE FUNCTION "oncreate_guilds"()
RETURNS TRIGGER
AS $$
BEGIN
    INSERT INTO "guild_ranks" ("name", "level", "guild_id") VALUES ('Leader', 3, NEW."id");
    INSERT INTO "guild_ranks" ("name", "level", "guild_id") VALUES ('Vice-Leader', 2, NEW."id");
    INSERT INTO "guild_ranks" ("name", "level", "guild_id") VALUES ('Member', 1, NEW."id");

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER "oncreate_guilds"
AFTER INSERT
ON "guilds"
FOR EACH ROW
EXECUTE PROCEDURE "oncreate_guilds"();

CREATE FUNCTION "oncreate_players"()
RETURNS TRIGGER
AS $$
BEGIN
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 0, 10);
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 1, 10);
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 2, 10);
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 3, 10);
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 4, 10);
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 5, 10);
    INSERT INTO "player_skills" ("player_id", "skillid", "value") VALUES (NEW."id", 6, 10);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER "oncreate_players"
AFTER INSERT
ON "players"
FOR EACH ROW
EXECUTE PROCEDURE "oncreate_players"();

CREATE FUNCTION "onupdate_players"()
RETURNS TRIGGER
AS $$
BEGIN
    IF OLD."lastlogin" < NEW."lastlogin" THEN
        DELETE FROM "player_spells" WHERE "player_id" = OLD."id";
        DELETE FROM "player_items" WHERE "player_id" = OLD."id";
        DELETE FROM "player_depotitems" WHERE "player_id" = OLD."id";
        DELETE FROM "player_storage" WHERE "player_id" = OLD."id";
        DELETE FROM "player_viplist" WHERE "player_id" = OLD."id";
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER "onupdate_players"
AFTER UPDATE
ON "players"
FOR EACH ROW
EXECUTE PROCEDURE "onupdate_players"();
