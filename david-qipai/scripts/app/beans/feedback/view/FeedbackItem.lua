local FAQItem = require("app.beans.help.view.FAQItem")
local FeedbackItem = class("FeedbackItem", FAQItem)
FeedbackItem.WIDTH = 750
FeedbackItem.HEIGHT = 64
function FeedbackItem:ctor()
  FAQItem.WIDTH = FeedbackItem.WIDTH
  FeedbackItem.super.ctor(self, FeedbackItem.WIDTH, FeedbackItem.HEIGHT)
end
function FeedbackItem:onDataSet(isModified, data)
  if isModified then
    data.question = g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 24, data.content, 680)
    data.answer = {}
    local answerItem = {}
    answerItem.type = "text"
    answerItem.isDimensions = true
    if #data.replies > 0 then
      answerItem.content = data.replies[1].content
    else
      answerItem.content = g.lang:getContent("feedback.noReply")
    end
    data.answer[1] = answerItem
    FeedbackItem.super.onDataSet(self, isModified, data)
  end
end
return FeedbackItem
