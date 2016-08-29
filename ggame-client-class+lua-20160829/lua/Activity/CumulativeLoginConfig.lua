--累计登陆奖励配置文件,d为天数，r为对应的奖励编号，配置在reward.txt中--
local CumulativeLoginConfig = {
        id = 27,
        DailyRewardItem = {
             [1] = {d = 1, 	r = 27001},
             [2] = {d = 2, 	r = 27002},
			 [3] = {d = 3, 	r = 27003},
			 [4] = {d = 4, 	r = 27004},
			 [5] = {d = 5, 	r = 27005},
			 [6] = {d = 6, 	r = 27006},
             [7] = {d = 7, 	r = 27007}
        },
        HELP = "HelpCumulativeLogin"
}

return CumulativeLoginConfig