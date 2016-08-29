-- gm用户表
-- 
CREATE DATABASE IF NOT EXISTS `dev_user`;
USE `dev_user`;

CREATE TABLE IF NOT EXISTS `user` (
    `u_name` varchar(40) NOT NULL COMMENT '用户名',
    `u_pass` varchar(50) NOT NULL COMMENT '密码',
    `u_ext`  varchar(50) DEFAULT '' COMMENT '备用',
    PRIMARY KEY (`u_name`)
)
COMMENT='gm操作用户表'
ENGINE=InnoDB DEFAULT CHARSET=utf8;