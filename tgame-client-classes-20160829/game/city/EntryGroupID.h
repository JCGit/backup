#ifndef EntryGroupID_h__
#define EntryGroupID_h__

typedef enum enumEntryGroupID {
	ENTRY_GROUP_UNKNOWN, // 未知
	ENTRY_GROUP_HEAD, // 头像下方
	ENTRY_GROUP_TOP, // 顶部
	ENTRY_GROUP_BOTTOM_H, // 底部横向
	ENTRY_GROUP_BOTTOM_V, // 底部纵向
	ENTRY_GROUP_SUB_GROWTH, // 成长
	ENTRY_GROUP_SUB_SOCIAL, // 社交
	ENTRY_GROUP_SUB_CHALLENGE, // 挑战
	ENTRY_GROUP_SUB_BUSINESS, // 商会
	ENTRY_GROUP_SUB_WEAPON, // 枪械
} EntryGroupID;

#endif // EntryGroupID_h__