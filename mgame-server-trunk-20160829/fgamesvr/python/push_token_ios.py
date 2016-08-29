#!/usr/bin/env python
#-*- coding: utf-8 -*-
'''

Copyright © 1998 - 2013 Tencent. All Rights Reserved. 腾讯公司 版权所有

'''

import sys
import xinge

# 以下iOS示例推送环境为开发环境
# iOS按标签推送
#def PushTokenIos(accessId,secretKey,content,token,environment)
xinge.PushTokenIos(2100145241, '37c1e7cb4edf243e2a5279315310dd04', sys.argv[2], sys.argv[1], xinge.XingeApp.ENV_DEV)
