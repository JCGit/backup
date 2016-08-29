# Config of server:
#   outer means server that could be accessed vis WAN;
#   inner means server that could only be accessed vis LAN;
#
# means of fileds:
#  inner|outer  WAN IP(or LAN IP for inner)   LAN IP           Memon          user    document root, ';' to split
#
#  NOTE: ALL fields could NOT have any **white space**.
SERVER_LIST=(\
    outer         '115.159.189.20'       '10.105.86.119'   'weixin中心服'     fgame      '/data/fgame/fgame' \
    outer         '115.159.43.94'        '10.105.104.244'  'weixin逻辑服'     fgame      '/data/fgame/fgame' \
	outer         '182.254.130.51'       '10.105.62.246'   'weixin逻辑服2'    fgame      '/data/fgame/fgame' \
    outer         '115.159.188.208'      '10.105.112.32'   'QQ中心服'         fgame      '/data/fgame/fgame' \
    outer         '115.159.187.190'      '10.105.118.6'    'QQ逻辑服'         fgame      '/data/fgame/fgame' \
	outer         '115.159.202.228'      '10.105.53.193'   'QQ逻辑服2'        fgame      '/data/fgame/fgame' \
    outer         '-'                    '10.105.66.183'   '排行服'           fgame      '/data/fgame/qq;/data/fgame/weixin' \
    inner         '10.10.1.112'          '10.10.1.112'     'TEST_SERVER_112'  root       '/root/net_fgame/trunk' \
    inner         '203.195.185.157'      '203.195.185.157' '腾讯兼容性测试服' root       '/root/net_fgame' )

## Log server is not in control since 2016/3/30
#    outer         '115.159.150.149'      '10.105.98.107'   '日志服'          fgame      '/data/fgame/trunk_qq;/data/fgame/trunk_weixin' \

# Master node of outer servers' deploying, local files will be uploaded
# to it, then it will deploy to all destinate OUTER server.
OUTER_MASTER='115.159.189.20'
OUTER_USER='fgame'
