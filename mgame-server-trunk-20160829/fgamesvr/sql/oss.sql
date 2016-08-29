-- MySQL dump 10.13  Distrib 5.1.73, for redhat-linux-gnu (x86_64)
--
-- Host: 183.61.111.199    Database: oss_fgame2_date
-- ------------------------------------------------------
-- Server version	5.5.42-cll-lve

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tb_oss_101_0`
--

DROP TABLE IF EXISTS `tb_oss_101_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_101_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `level` int(11) NOT NULL COMMENT '级别',
  `login_type` int(11) NOT NULL COMMENT '登陆类别 1 上线 0 下线',
  `online_time` int(11) NOT NULL COMMENT '在线时长',
  `login_time` int(11) NOT NULL COMMENT '登陆时间',
  `loginout_time` int(11) NOT NULL COMMENT '登出时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1384 DEFAULT CHARSET=utf8 COMMENT='登陆登出';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_101` AFTER INSERT ON `tb_oss_101_0` FOR EACH ROW BEGIN
insert into oss_project.oss_login_user(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,level,login_type,online_time,login_time,loginout_time)  
select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,level,login_type,online_time,login_time,loginout_time from oss_fgame_data.tb_oss_101_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_102_0`
--

DROP TABLE IF EXISTS `tb_oss_102_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_102_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(255) DEFAULT '' COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `room_id` int(11) NOT NULL COMMENT '房间ID',
  `battle_type` int(11) NOT NULL COMMENT '战斗类型 1:PVE  2:2v2 3:3V3 4:4V4 20WIFI',
  `hero_id` int(11) NOT NULL COMMENT '英雄id',
  `result` int(11) NOT NULL COMMENT '战斗结果1 胜利 0 失败',
  `used_ap` int(11) NOT NULL COMMENT '体力消耗',
  `map_id` int(11) NOT NULL COMMENT '地图id',
  `battle_time` int(11) NOT NULL COMMENT '战斗时长',
  `begin_time` int(11) NOT NULL COMMENT '开始时间',
  `finish_time` int(11) NOT NULL COMMENT '结束时间',
  `star` int(11) NOT NULL DEFAULT '0' COMMENT 'pve通关星数',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=925 DEFAULT CHARSET=utf8 COMMENT='玩家行为PVE/PVP';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_102` AFTER INSERT ON `tb_oss_102_0` FOR EACH ROW BEGIN
insert into oss_project.oss_pve_pvp(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,room_id,battle_type,hero_id,result,used_ap,map_id,battle_time,begin_time,finish_time,star)  
select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,room_id,battle_type,hero_id,result,used_ap,map_id,battle_time,begin_time,finish_time,star from oss_fgame_data.tb_oss_102_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_103_0`
--

DROP TABLE IF EXISTS `tb_oss_103_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_103_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `level` int(11) NOT NULL COMMENT '级别',
  `currency_type` int(11) NOT NULL COMMENT '货币类型1 钻石 2 金币 3 积分',
  `change_type` int(11) NOT NULL DEFAULT '0' COMMENT '变化类型(货币来源) 1,增加，2减少',
  `change_value` int(11) NOT NULL COMMENT '变化值',
  `currency` int(11) NOT NULL COMMENT '货币余额',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=528 DEFAULT CHARSET=utf8 COMMENT='货币变化';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_103` AFTER INSERT ON `tb_oss_103_0` FOR EACH ROW BEGIN
insert into oss_project.oss_change_money(game_id ,area ,city_code,type,pid,user_name,zone_id,platform_type,record_time,level,currency_type,change_type,change_value,currency)
select game_id ,area ,city_code,type,pid,user_name,zone_id,platform_type,record_time,level,currency_type,change_type,change_value,currency from oss_fgame_data.tb_oss_103_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_104_0`
--

DROP TABLE IF EXISTS `tb_oss_104_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_104_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `level` int(11) NOT NULL COMMENT '级别',
  `item_serial` int(11) NOT NULL COMMENT '物品序号',
  `item_id` int(11) NOT NULL COMMENT '物品id',
  `change_value` int(11) NOT NULL COMMENT '变化值',
  `change_type` int(11) NOT NULL COMMENT '变化类型',
  `item_source` int(11) NOT NULL COMMENT '来源',
  `item_counts` int(11) NOT NULL COMMENT '物品数量',
  `gold_change` int(11) NOT NULL COMMENT '金币变化',
  `diamond_change` int(11) NOT NULL COMMENT '钻石变化',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4921 DEFAULT CHARSET=utf8 COMMENT='物品变化';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_104` AFTER INSERT ON `tb_oss_104_0` FOR EACH ROW BEGIN
insert into oss_project.oss_stock_data(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,level,item_serial,item_id,change_value,change_type,item_source,item_counts,gold_change,diamond_change)
select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,level,item_serial,item_id,change_value,change_type,item_source,item_counts,gold_change,diamond_change from oss_fgame_data.tb_oss_104_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_105_0`
--

DROP TABLE IF EXISTS `tb_oss_105_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_105_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `name` varchar(512) NOT NULL COMMENT '昵称',
  `level` int(11) NOT NULL COMMENT '级别',
  `vip_level` int(11) NOT NULL COMMENT 'VIP级别',
  `pvp_score` int(11) NOT NULL COMMENT '战斗力',
  `gold` int(11) NOT NULL COMMENT '变化值',
  `diamonds` int(11) NOT NULL COMMENT '变化类型',
  `newbie_flag` int(11) NOT NULL COMMENT '新手标志',
  `regist_time` int(11) NOT NULL COMMENT '注册时间',
  `frt_recharge_time` int(11) NOT NULL COMMENT '首次充值时间',
  `three_login_times` varchar(512) NOT NULL COMMENT '前三次登录时间',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1133 DEFAULT CHARSET=utf8 COMMENT='玩家信息';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_105` AFTER INSERT ON `tb_oss_105_0` FOR EACH ROW BEGIN
select count(id),id INTO @flag,@oss_id  from oss_project.oss_reg_user as oss where oss.game_id=new.game_id and oss.area=new.area and oss.pid=new.pid and oss.user_name=new.user_name and oss.zone_id=new.zone_id and oss.platform_type=new.platform_type;
-- 3.无则插入
 if @flag=0 then
 insert into oss_project.oss_reg_user(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,name,level,vip_level,pvp_score,gold,diamonds,newbie_flag,regist_time,frt_recharge_time,three_login_times) 
  select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,name,level,vip_level,pvp_score,gold,diamonds,newbie_flag,regist_time,frt_recharge_time,three_login_times from oss_fgame_data.tb_oss_105_0 where id=new.id;
 else
    -- 4.有则更新
   update oss_project.oss_reg_user set record_time=UNIX_TIMESTAMP(new.record_time),name=new.name,level=new.level,vip_level=new.vip_level,pvp_score=new.pvp_score,gold=new.gold,diamonds=new.diamonds,newbie_flag=new.newbie_flag,frt_recharge_time=new.frt_recharge_time,three_login_times=new.three_login_times where id=@oss_id and pid=new.pid;
 end if;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_106_0`
--

DROP TABLE IF EXISTS `tb_oss_106_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_106_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` int(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `items` varchar(1024) DEFAULT NULL COMMENT '道具json数据',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1060 DEFAULT CHARSET=utf8 COMMENT='背包';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_106` AFTER INSERT ON `tb_oss_106_0` FOR EACH ROW BEGIN
insert into oss_project.oss_bag(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,items) 
select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,items from oss_fgame_data.tb_oss_106_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_107_0`
--

DROP TABLE IF EXISTS `tb_oss_107_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_107_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `serial` int(11) NOT NULL COMMENT '统计序号',
  `online` int(11) NOT NULL COMMENT '在线人数',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1480 DEFAULT CHARSET=utf8 COMMENT='在线人数';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_107` AFTER INSERT ON `tb_oss_107_0` FOR EACH ROW BEGIN
insert into oss_project.oss_online_5min(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,serial,online) 
select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,serial,online from oss_fgame_data.tb_oss_107_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_108_0`
--

DROP TABLE IF EXISTS `tb_oss_108_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_108_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `trade_no` varchar(512) NOT NULL COMMENT '交易流水号',
  `product_id` varchar(512) NOT NULL COMMENT '商品ID',
  `currency_type` int(11) NOT NULL COMMENT '货币类型',
  `total_fee` float(11,2) NOT NULL COMMENT '消费金额',
  `channel_type` int(11) NOT NULL COMMENT '渠道类型',
  `trade_time` int(11) NOT NULL COMMENT '交易时间',
  `diamonds` int(11) NOT NULL COMMENT '购买钻石数',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='充值记录';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`web`@`%`*/ /*!50003 TRIGGER `oss_108` AFTER INSERT ON `tb_oss_108_0` FOR EACH ROW BEGIN
insert into oss_project.oss_pay_user(game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,trade_no,product_id,currency_type,total_fee,channel_type,trade_time,diamonds)
select game_id,area,city_code,type,pid,user_name,zone_id,platform_type,record_time,trade_no,product_id,currency_type,total_fee,channel_type,trade_time,diamonds from oss_fgame_data.tb_oss_108_0 where id=new.id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `tb_oss_109_0`
--

DROP TABLE IF EXISTS `tb_oss_109_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_109_0` (
  `id` bigint(11) unsigned NOT NULL AUTO_INCREMENT COMMENT '记录编号，自动递增',
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `activity_id` varchar(255) DEFAULT NULL COMMENT '活动id',
  `activity_name` varchar(255) NOT NULL DEFAULT '' COMMENT '活动名称',
  `content` varchar(255) DEFAULT NULL COMMENT '活动数据,签到存签到天数，连胜存连胜数',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=96 DEFAULT CHARSET=utf8 COMMENT='活动记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tb_oss_110_0`
--

DROP TABLE IF EXISTS `tb_oss_110_0`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_oss_110_0` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `game_id` int(11) unsigned NOT NULL COMMENT '游戏id',
  `area` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '地区代码',
  `city_code` char(20) NOT NULL COMMENT '城市代码',
  `type` int(11) unsigned NOT NULL COMMENT '类型',
  `pid` bigint(11) unsigned NOT NULL COMMENT '玩家id',
  `user_name` char(38) NOT NULL COMMENT '用户名/账号id',
  `zone_id` int(11) unsigned NOT NULL COMMENT '大区id',
  `platform_type` int(11) unsigned NOT NULL COMMENT '平台类型',
  `note` varchar(1024) NOT NULL COMMENT '描述',
  `record_time` int(11) DEFAULT NULL COMMENT '记录时间',
  `achievement_type` int(11) NOT NULL DEFAULT '0' COMMENT '1,完成成就，2领取成就',
  `achievement_id` int(11) NOT NULL DEFAULT '0' COMMENT '成就id',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=96 DEFAULT CHARSET=utf8 COMMENT='完成成就表';
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-31 17:12:32
