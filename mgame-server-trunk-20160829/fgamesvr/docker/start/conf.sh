#!/bin/bash

# with this format
# ipaddr describe director(volumn)
SERVER_LIST=(\
    10.10.2.54    '测试环境54'    '/home/game/dev54'  'dev54' \
    10.10.2.88    '测试环境88'    '/home/game/dev88'  'dev88' \
    10.10.2.89    '测试环境89'    '/home/game/dev89'  'dev89' \
    10.10.2.90    '测试环境90'    '/home/game/dev90'  'dev90' \
    10.10.2.91    '测试环境91'    '/home/game/dev91'  'dev91' \
    )

# exposed port
SERVER_EXPOSE_PORT=(\
    3306 \
    6379 \
    9000 \
    9004 \
    7602 \
    7777)

# 7777 是ssh映射端口
