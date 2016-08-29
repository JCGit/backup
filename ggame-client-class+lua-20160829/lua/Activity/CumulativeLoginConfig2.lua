--累计登陆奖励配置文件,d为天数，r为对应的奖励编号，配置在reward.txt中--
local CumulativeLoginConfig = {
        id = 32,
        DailyRewardItem = {
             [1] = {d = 1, 	r = 28001},
             [2] = {d = 2, 	r = 28002},
			 [3] = {d = 3, 	r = 28003},
			 [4] = {d = 4, 	r = 28004},
			 [5] = {d = 5, 	r = 28005},
			 [6] = {d = 6, 	r = 28006},
             [7] = {d = 7, 	r = 28007}
        },
        HELP = "HelpCumulativeLogin"
}

return CumulativeLoginConfig