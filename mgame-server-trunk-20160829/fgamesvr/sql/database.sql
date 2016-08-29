-- 数据库表
-- 表的规则： 只有两个字段 第一个字段 id 第二个字段是id对应的对象 可以是一个值 也可以是序列化的对象

CREATE DATABASE IF NOT EXISTS `fgame`;
USE `fgame`;

CREATE TABLE IF NOT EXISTS `account` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
	`account_info` LONGTEXT NOT NULL COMMENT '账号信息',
	PRIMARY KEY (`id`)
)
COMMENT='账号表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `point` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
	`point_data` LONGTEXT NOT NULL COMMENT '关卡数据',
	PRIMARY KEY (`id`)
)
COMMENT='关卡表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `uin_mapping` (
	`id` varchar(128) NOT NULL COMMENT 'openid',
	`uin` varchar(128) NOT NULL COMMENT '账号ID',
	PRIMARY KEY (`id`)
)
COMMENT='openid与uin映射表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `achievement` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
	`achievement_data`  LONGTEXT NOT NULL COMMENT '成就数据',
	PRIMARY KEY (`id`)
)
COMMENT='角色成就表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `item` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
	`item_data`  LONGTEXT NOT NULL COMMENT '道具数据',
	PRIMARY KEY (`id`)
)
COMMENT='角色道具表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `rank` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '排名数据',
    PRIMARY KEY (`id`)
)
COMMENT='排行榜'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `seven_day_sign` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '签到数据',
    PRIMARY KEY (`id`)
)
COMMENT='7日签到'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `xinge` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
	`xinge_info` LONGTEXT NOT NULL COMMENT '信鸽信息',
	PRIMARY KEY (`id`)
)
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `achievement_info` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '成就ID',
	`achievement_info` LONGTEXT NOT NULL COMMENT '成就信息',
	PRIMARY KEY (`id`)
)
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `player_name` (
	`id` varchar(20) NOT NULL COMMENT 'name',
	`name` LONGTEXT NOT NULL COMMENT 'name信息',
	PRIMARY KEY (`id`)
)
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `power_energy_activity` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT 'uin',
	`power_energy_info` LONGTEXT NOT NULL COMMENT '活动信息',
	PRIMARY KEY (`id`)
)
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `mission` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '任务数据',
    PRIMARY KEY (`id`)
)
COMMENT='任务'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `activity_info` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `activity_info` LONGTEXT NOT NULL COMMENT '活动数据',
    PRIMARY KEY (`id`)
)
COMMENT='活动数据'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `newbie_gift` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `newbie_gift` LONGTEXT NOT NULL COMMENT '新手礼包数据',
    PRIMARY KEY (`id`)
)
COMMENT='新手礼包'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `recharge_info` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `recharge_info` LONGTEXT NOT NULL COMMENT '充值数据',
    PRIMARY KEY (`id`)
)
COMMENT='充值数据'
ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `player_gift_code` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `gift_code` LONGTEXT NOT NULL COMMENT '礼包数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家礼包激活数据'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `gift_code` (
    `id` varchar(20) NOT NULL COMMENT '礼包激活码',
    `gift_code` LONGTEXT NOT NULL COMMENT '礼包数据',
    PRIMARY KEY (`id`)
)
COMMENT='礼包激活数据存档'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `gift_type` (
    `id` varchar(20) NOT NULL COMMENT '礼包类型',
    `gift_type` LONGTEXT NOT NULL COMMENT '礼包类型数据',
    PRIMARY KEY (`id`)
)
COMMENT='礼包类型数据存档'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `recharge_order` (
    `id` varchar(255) NOT NULL COMMENT '充值订单数据key',
    `recharge_order` LONGTEXT NOT NULL COMMENT '充值订单数据',
    PRIMARY KEY (`id`)
)
COMMENT='充值订单数据'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `player_sync_data` (
     `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `player_sync_data` LONGTEXT NOT NULL COMMENT '玩家数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家数据'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `player_recharge_gift` (
     `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `player_recharge_gift` LONGTEXT NOT NULL COMMENT '玩家充值附送数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家充值附送数据'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `friend_info` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `friend_info` LONGTEXT NOT NULL COMMENT '玩家好友信息',
    PRIMARY KEY (`id`)
)
COMMENT='好友信息表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `global_mail` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '邮件ID',
    `data` LONGTEXT NOT NULL COMMENT '邮件信息',
    PRIMARY KEY (`id`)
)
COMMENT='全服邮件'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `usermail` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '邮件信息',
    PRIMARY KEY (`id`)
)
COMMENT='用户邮件表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `single_mail` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '邮件id',
    `data` LONGTEXT NOT NULL COMMENT '邮件信息',
    PRIMARY KEY (`id`)
)
COMMENT='单点邮件'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `check_points` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '关卡信息',
    PRIMARY KEY (`id`)
)
COMMENT='用户关卡表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `exchange_info` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '荣誉值兑换信息',
    PRIMARY KEY (`id`)
)
COMMENT='用户荣誉值兑换信息表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `glyphData` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '玩家雕文数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家雕文数据表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `stuffData` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
    `data` LONGTEXT NOT NULL COMMENT '玩家雕文材料数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家雕文材料表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `player_titles` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '勋章ID',
    `data` LONGTEXT NOT NULL COMMENT '勋章数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家勋章称号表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `newbie_list` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '新手指引ID',
    `data` LONGTEXT NOT NULL COMMENT '新手指引数据',
    PRIMARY KEY (`id`)
)
COMMENT='玩家新手指引表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `unique_code_factory` (
    `id` VARCHAR(128) NOT NULL COMMENT '唯一码名字',
    `data` LONGTEXT NOT NULL COMMENT '唯一码生成信息',
    PRIMARY KEY (`id`)
)
COMMENT='唯一码生成工厂表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `new_year_info` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '玩家uin',
    `data` LONGTEXT NOT NULL COMMENT '玩家元旦活动信息',
    PRIMARY KEY (`id`)
)
COMMENT='元旦活动信息表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `invite_code_mapping` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '邀请码',
    `data` LONGTEXT NOT NULL COMMENT '邀请码对应玩家uin',
    PRIMARY KEY (`id`)
)
COMMENT='邀请码对用玩家uin表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `seven_goal` (
    `id` BIGINT UNSIGNED NOT NULL COMMENT '玩家uin',
    `data` LONGTEXT NOT NULL COMMENT '七日目标数据',
    PRIMARY KEY (`id`)
)
COMMENT='七日目标'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `christmas_data` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '账号ID',
	`data` LONGTEXT NOT NULL COMMENT '圣诞活动数据',
	PRIMARY KEY (`id`)
)
COMMENT='圣诞活动数据表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `operate_activities` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '活动id',
	`data` LONGTEXT NOT NULL COMMENT '活动配置',
	PRIMARY KEY (`id`)
)
COMMENT='运营活动数据表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `activityGlobalData` (
	`id` BIGINT UNSIGNED NOT NULL COMMENT '活动id',
	`data` LONGTEXT NOT NULL COMMENT '全局活动数据',
	PRIMARY KEY (`id`)
)
COMMENT='全局活动数据表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;
