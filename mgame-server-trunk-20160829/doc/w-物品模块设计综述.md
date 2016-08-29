##物品模块设计综述
	auth:diandian
	date:2016.4.8


##功能概述
* 统计玩家相关的物品数量，包括玩家属性抽象成的物品id
* 转发合作模块物品改变信息
* 相关联物品的添加触发，减少触发，持有抵触等各种判断

##设计概述
物品模块的设计旨在让玩家的物品（包括背包的，和非背包的）有一个统一的管理和调用接口。模块间物品改变时由事件通知到感兴趣的模块，以作相应的处理。 物品之间有一个持有等级和唯一性的属性，唯一性的物品同类型的增加，持有等级高的会抵触持有等级低的。持有等级高的会之间将等级低的替换，等级低的在有等级高的时候则会不能持有。

##api接口说明
* `agent_item.add_item(itemid, count, src, notice_flag, extra)`:
  主动调用添加物品数量, 会触发物品增加事件,
* `agent_item.reduce_item(itemid, count, src, notice_flag, extra)`:
  主动调用减少物品数量, 会出发物品减少事件
* `agent_item.use_item(itemid, count, src, notice_flag)`:
  主动调用使用物品，物品一旦被使用将减少其数量，并且会出发相关使用效果
* `agent_item.item_on_add(id, count, src, notice_flag)`:
  当合作模块需要维护物品数量增加时调用，不会触发物品改变的事件
* `agent_item.item_on_reduce(id, count, src, notice_flag)`:
  当合作模块需要维护物品数量减少时调用


