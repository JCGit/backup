DEBUG = 0
DEBUG_FPS = false
DEBUG_MEM = false
LOAD_DEPRECATED_API = false
LOAD_SHORTCODES_API = true
CONFIG_SCREEN_ORIENTATION = "landscape"
CONFIG_SCREEN_WIDTH = 960
CONFIG_SCREEN_HEIGHT = 640
local openGLView = CCDirector:sharedDirector():getOpenGLView()
local openGLSize = openGLView:getFrameSize()
local width = openGLSize.width
local height = openGLSize.height
if width / height >= CONFIG_SCREEN_WIDTH / CONFIG_SCREEN_HEIGHT then
  CONFIG_SCREEN_AUTOSCALE = "FIXED_HEIGHT"
else
  CONFIG_SCREEN_AUTOSCALE = "FIXED_WIDTH"
end
DEBUG_FAKE_JSON = false
DEBUG_MEMORY_STATUS = false
DEBUG_RECORD_MODE = false
