DROP TRIGGER IF EXISTS `oncreate_players`;
DROP TRIGGER IF EXISTS `oncreate_guilds`;
DROP TRIGGER IF EXISTS `ondelete_players`;
DROP TRIGGER IF EXISTS `ondelete_guilds`;
DROP TRIGGER IF EXISTS `ondelete_accounts`;

DROP TABLE IF EXISTS `player_depotitems`;
DROP TABLE IF EXISTS `tile_items`;
DROP TABLE IF EXISTS `tiles`;
DROP TABLE IF EXISTS `bans`;
DROP TABLE IF EXISTS `house_lists`;
DROP TABLE IF EXISTS `houses`;
DROP TABLE IF EXISTS `player_items`;
DROP TABLE IF EXISTS `player_skills`;
DROP TABLE IF EXISTS `player_storage`;
DROP TABLE IF EXISTS `player_viplist`;
DROP TABLE IF EXISTS `player_spells`;
DROP TABLE IF EXISTS `player_deaths`;
DROP TABLE IF EXISTS `guild_ranks`;
DROP TABLE IF EXISTS `guilds`;
DROP TABLE IF EXISTS `guild_invites`;
DROP TABLE IF EXISTS `global_storage`;
DROP TABLE IF EXISTS `players`;
DROP TABLE IF EXISTS `accounts`;
DROP TABLE IF EXISTS `groups`;
DROP TABLE IF EXISTS `server_record`;
DROP TABLE IF EXISTS `server_motd`;
DROP TABLE IF EXISTS `server_reports`;
DROP TABLE IF EXISTS `server_config`;

CREATE TABLE `server_config`
(
	`config` VARCHAR(35) NOT NULL DEFAULT '',
	`value` INT NOT NULL
) ENGINE = InnoDB;

INSERT INTO `server_config` VALUES ('db_version', 1);

CREATE TABLE `server_motd`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`text` TEXT NOT NULL,
	PRIMARY KEY (`id`)
) ENGINE = InnoDB;

INSERT INTO `server_motd` VALUES (1, 'Welcome to The Forgotten Server!');

CREATE TABLE `server_record`
(
	`record` INT NOT NULL,
	`timestamp` BIGINT NOT NULL,
	PRIMARY KEY (`timestamp`)
) ENGINE = InnoDB;

INSERT INTO `server_record` VALUES (0, 0);

CREATE TABLE `server_reports`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`player_id` INT UNSIGNED NOT NULL DEFAULT 0,
	`posx` INT NOT NULL DEFAULT 0,
	`posy` INT NOT NULL DEFAULT 0,
	`posz` INT NOT NULL DEFAULT 0,
	`timestamp` BIGINT NOT NULL DEFAULT 0,
	`report` TEXT NOT NULL,
	`reads` INT NOT NULL DEFAULT 0,
	PRIMARY KEY (`id`),
	KEY (`player_id`),
	KEY (`reads`)
) ENGINE = InnoDB;

CREATE TABLE `groups`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(255) NOT NULL COMMENT 'group name',
	`flags` BIGINT UNSIGNED NOT NULL DEFAULT 0,
	`customflags` BIGINT UNSIGNED NOT NULL DEFAULT 0,
	`access` INT NOT NULL,
	`maxdepotitems` INT NOT NULL,
	`maxviplist` INT NOT NULL,
	PRIMARY KEY (`id`)
) ENGINE = InnoDB;

INSERT INTO `groups` VALUES (1, 'Player', 0, 0, 0, 0, 0);
INSERT INTO `groups` VALUES (2, 'Tutor', 16809984, 1, 1, 0, 0);
INSERT INTO `groups` VALUES (3, 'Senior Tutor', 68736352256, 3, 2, 0, 0);
INSERT INTO `groups` VALUES (4, 'Game Master', 492842123151, 63, 3, 4000, 200);
INSERT INTO `groups` VALUES (5, 'Community Manager', 542239465466, 1279, 4, 6000, 300);
INSERT INTO `groups` VALUES (6, 'God', 546534563834, 2047, 5, 8000, 400);


CREATE TABLE `accounts`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`password` VARCHAR(255) NOT NULL/* VARCHAR(32) NOT NULL COMMENT 'MD5'*//* VARCHAR(40) NOT NULL COMMENT 'SHA1'*/,
	`premdays` INT NOT NULL DEFAULT 0,
	`lastday` INT UNSIGNED NOT NULL DEFAULT 0,
	`email` VARCHAR(255) NOT NULL DEFAULT '',
	`key` VARCHAR(20) NOT NULL DEFAULT '0',
	`blocked` TINYINT(1) NOT NULL DEFAULT FALSE,
	`warnings` INT NOT NULL DEFAULT 0,
	`group_id` INT NOT NULL DEFAULT 1,
	`deleted` TINYINT(1) NOT NULL DEFAULT FALSE,
	`warned` TINYINT(1) NOT NULL DEFAULT FALSE,
	PRIMARY KEY (`id`),
	FOREIGN KEY (`group_id`) REFERENCES `groups` (`id`)
) ENGINE = InnoDB;

INSERT INTO `accounts` VALUES (1, '1', 65535, 0, '', '0', 0, 0, 1, 0, 0);
INSERT INTO `accounts` VALUES (2, '2', 0, 0, '', '0', 0, 0, 6, 0, 0);

CREATE TABLE `players`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(255) NOT NULL,
	`group_id` INT NOT NULL DEFAULT 1,
	`account_id` INT NOT NULL DEFAULT 0,
	`level` INT NOT NULL DEFAULT 1,
	`vocation` INT NOT NULL DEFAULT 0,
	`health` INT NOT NULL DEFAULT 150,
	`healthmax` INT NOT NULL DEFAULT 150,
	`experience` BIGINT NOT NULL DEFAULT 0,
	`lookbody` INT NOT NULL DEFAULT 0,
	`lookfeet` INT NOT NULL DEFAULT 0,
	`lookhead` INT NOT NULL DEFAULT 0,
	`looklegs` INT NOT NULL DEFAULT 0,
	`looktype` INT NOT NULL DEFAULT 136,
	`maglevel` INT NOT NULL DEFAULT 0,
	`mana` INT NOT NULL DEFAULT 0,
	`manamax` INT NOT NULL DEFAULT 0,
	`manaspent` INT NOT NULL DEFAULT 0,
	`soul` INT UNSIGNED NOT NULL DEFAULT 0,
	`town_id` INT NOT NULL DEFAULT 0,
	`posx` INT NOT NULL DEFAULT 0,
	`posy` INT NOT NULL DEFAULT 0,
	`posz` INT NOT NULL DEFAULT 0,
	`conditions` BLOB NOT NULL,
	`cap` INT NOT NULL DEFAULT 0,
	`sex` INT NOT NULL DEFAULT 0,
	`lastlogin` BIGINT UNSIGNED NOT NULL DEFAULT 0,
	`lastip` INT UNSIGNED NOT NULL DEFAULT 0,
	`save` TINYINT(1) NOT NULL DEFAULT 1,
	`redskull` TINYINT(1) NOT NULL DEFAULT 0,
	`redskulltime` BIGINT NOT NULL DEFAULT 0,
	`rank_id` INT NOT NULL DEFAULT 0,
	`guildnick` VARCHAR(255) NOT NULL DEFAULT '',
	`lastlogout` BIGINT UNSIGNED NOT NULL DEFAULT 0,
	`blessings` TINYINT(2) NOT NULL DEFAULT 0,
	`balance` BIGINT NOT NULL DEFAULT 0 COMMENT 'player money in bank',
	`direction` INT NOT NULL DEFAULT 2 COMMENT 'NOT IN USE BY THE SERVER',
	`loss_experience` INT NOT NULL DEFAULT 10,
	`loss_mana` INT NOT NULL DEFAULT 10,
	`loss_skills` INT NOT NULL DEFAULT 10,
	`loss_items` INT NOT NULL DEFAULT 10,
	`online` TINYINT NOT NULL DEFAULT 0,
	`marriage` INT UNSIGNED NOT NULL DEFAULT 0,
	PRIMARY KEY (`id`),
	KEY (`name`),
	FOREIGN KEY (`account_id`) REFERENCES `accounts`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`group_id`) REFERENCES `groups`(`id`)
) ENGINE = InnoDB;

INSERT INTO `players` VALUES (1, 'Account Manager', 1, 1, 1, 0, 150, 150, 0, 0, 0, 0, 0, 110, 0, 0, 0, 0, 0, 1, 33210, 31844, 7, '', 400, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 0, 0, 10, 10, 10, 10, 0, 0);
INSERT INTO `players` VALUES (2, 'GOD Tryller', 6, 1, 1, 0, 150, 150, 0, 0, 0, 0, 0, 74, 0, 0, 0, 0, 0, 1, 33210, 31844, 7, '', 400, 0, 0, 0, 1, 0, 0, 0, '', 0, 0, 0, 0, 10, 10, 10, 10, 0, 0);

CREATE TABLE `bans` 
(
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `type` INT NOT NULL COMMENT 'this field defines if its ip, account, player, or any else ban',
    `value` INT UNSIGNED NOT NULL COMMENT 'ip, player guid, account number',
    `param` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'mask',
    `active` TINYINT(1) NOT NULL DEFAULT TRUE,
    `expires` INT UNSIGNED NOT NULL,
    `added` INT UNSIGNED NOT NULL,
    `admin_id` INT UNSIGNED NOT NULL DEFAULT 0,
    `reason` VARCHAR(255) NOT NULL DEFAULT '',
    `comment` VARCHAR(255) NOT NULL DEFAULT '',
    PRIMARY KEY  (`id`),
    KEY (`type`, `value`),
    KEY (`expires`)
) ENGINE = InnoDB;

CREATE TABLE `global_storage`
(
	`key` INT UNSIGNED NOT NULL,
	`value` INT NOT NULL,
	PRIMARY KEY  (`key`)
) ENGINE = InnoDB;

CREATE TABLE `guilds`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(255) NOT NULL COMMENT 'guild name - nothing else needed here',
	`ownerid` INT NOT NULL,
	`creationdata` INT NOT NULL,
	`motd` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`id`)
) ENGINE = InnoDB;

CREATE TABLE `guild_invites`
(
	`player_id` INT UNSIGNED NOT NULL DEFAULT 0,
	`guild_id` INT UNSIGNED NOT NULL DEFAULT 0
) ENGINE = InnoDB;

CREATE TABLE `guild_ranks`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`guild_id` INT NOT NULL COMMENT 'guild',
	`name` VARCHAR(255) NOT NULL COMMENT 'rank name',
	`level` INT NOT NULL COMMENT 'rank level - leader, vice, member, maybe something else',
	PRIMARY KEY (`id`),
	FOREIGN KEY (`guild_id`) REFERENCES `guilds`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `house_lists`
(
	`house_id` INT NOT NULL,
	`listid` INT NOT NULL,
	`list` TEXT NOT NULL
) ENGINE = InnoDB;

CREATE TABLE `houses`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`owner` INT NOT NULL,
	`paid` INT UNSIGNED NOT NULL DEFAULT 0,
	`warnings` INT NOT NULL DEFAULT 0,
	`lastwarning` INT UNSIGNED NOT NULL DEFAULT 0,
	PRIMARY KEY (`id`)
) ENGINE = InnoDB;

CREATE TABLE `player_deaths`
(
	`player_id` INT NOT NULL,
	`time` BIGINT UNSIGNED NOT NULL DEFAULT 0,
	`level` INT NOT NULL DEFAULT 1,
	`killed_by` VARCHAR(255) NOT NULL,
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `player_depotitems`
(
	`player_id` INT NOT NULL,
	`depot_id` INT NOT NULL DEFAULT 0,
	`sid` INT NOT NULL COMMENT 'any given range eg 0-100 will be reserved for depot lockers and all > 100 will be then normal items inside depots',
	`pid` INT NOT NULL DEFAULT 0,
	`itemtype` INT NOT NULL,
	`count` INT NOT NULL DEFAULT 0,
	`attributes` BLOB NOT NULL,
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE,
	KEY (`player_id`, `depot_id`),
	UNIQUE KEY (`player_id`, `sid`)
) ENGINE = InnoDB;

CREATE TABLE `player_items`
(
	`player_id` INT NOT NULL DEFAULT 0,
	`pid` INT NOT NULL DEFAULT 0,
	`sid` INT NOT NULL DEFAULT 0,
	`itemtype` INT NOT NULL DEFAULT 0,
	`count` INT NOT NULL DEFAULT 0,
	`attributes` text NOT NULL,
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `player_skills`
(
	`player_id` INT NOT NULL DEFAULT 0,
	`skillid` tinyint(4) NOT NULL DEFAULT 0,
	`value` INT UNSIGNED NOT NULL DEFAULT 0,
	`count` INT UNSIGNED NOT NULL DEFAULT 0,
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `player_spells`
(
	`player_id` INT NOT NULL,
	`name` VARCHAR(255) NOT NULL,
	 FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `player_storage`
(
	`player_id` INT NOT NULL DEFAULT 0,
	`key` INT UNSIGNED NOT NULL DEFAULT 0,
	`value` INT NOT NULL DEFAULT 0,
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `player_viplist`
(
	`player_id` INT NOT NULL COMMENT 'id of player whose viplist entry it is',
	`vip_id` INT NOT NULL COMMENT 'id of target player of viplist entry',
	FOREIGN KEY (`player_id`) REFERENCES `players`(`id`) ON DELETE CASCADE,
	FOREIGN KEY (`vip_id`) REFERENCES `players`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

CREATE TABLE `tiles`
(
	`id` INT NOT NULL AUTO_INCREMENT,
	`x` INT NOT NULL,
	`y` INT NOT NULL,
	`z` INT NOT NULL,
	PRIMARY KEY(`id`)
) ENGINE = InnoDB;

CREATE TABLE `tile_items`
(
	`tile_id` INT NOT NULL,
	`sid` INT NOT NULL,
	`pid` INT NOT NULL DEFAULT 0,
	`itemtype` INT NOT NULL,
	`count` INT NOT NULL DEFAULT 0,
	`attributes` BLOB NOT NULL,
	FOREIGN KEY (`tile_id`) REFERENCES `tiles`(`id`) ON DELETE CASCADE
) ENGINE = InnoDB;

DELIMITER |

CREATE TRIGGER `ondelete_accounts`
BEFORE DELETE
ON `accounts`
FOR EACH ROW
BEGIN
    DELETE FROM `bans` WHERE `type` != 1 AND `type` != 2 AND `value` = OLD.`id`;
END|

CREATE TRIGGER `ondelete_guilds`
BEFORE DELETE
ON `guilds`
FOR EACH ROW
BEGIN
    UPDATE `players` SET `guildnick` = '', `rank_id` = 0 WHERE `rank_id` IN (SELECT `id` FROM `guild_ranks` WHERE `guild_id` = OLD.`id`);
END|

CREATE TRIGGER `ondelete_players`
BEFORE DELETE
ON `players`
FOR EACH ROW
BEGIN
    DELETE FROM `bans` WHERE `type` = 2 AND `value` = OLD.`id`;
    UPDATE `houses` SET `owner` = 0 WHERE `owner` = OLD.`id`;
END|

CREATE TRIGGER `oncreate_guilds`
AFTER INSERT
ON `guilds`
FOR EACH ROW
BEGIN
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('the Leader', 3, NEW.`id`);
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('a Vice-Leader', 2, NEW.`id`);
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('a Member', 1, NEW.`id`);
END|

CREATE TRIGGER `oncreate_players`
AFTER INSERT
ON `players`
FOR EACH ROW
BEGIN
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 0, 10);
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 1, 10);
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 2, 10);
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 3, 10);
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 4, 10);
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 5, 10);
    INSERT INTO `player_skills` (`player_id`, `skillid`, `value`) VALUES (NEW.`id`, 6, 10);
END|

DELIMITER ;
