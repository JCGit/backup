local AudioManager = class("AudioManager")
AudioManager.sound = {}
AudioManager.music = {
  DEFAULT = "voice/pd_bgm"
}
AudioManager.sound.commonSounds = {
  CLICK_BUTTON = "voice/pd_button_click",
  POPUP_OPEN = "voice/pd_popup_open",
  POPUP_CLOSE = "voice/pd_popup_close",
  SCENE_SWITCH = "voice/pd_scene_switch",
  REWARD_GET = "voice/pd_reward_get",
  MESSAGE_SEND = "voice/pd_message_send"
}
AudioManager.sound.tableSounds = {
  SIT_DOWN = "voice/pd_table_sit",
  BET = "voice/pd_table_bet",
  TAP_TABLE = "voice/pd_table_taptable",
  DEAL_CARD = "voice/pd_table_dealcard",
  MOVE_CHIP = "voice/pd_table_chipmove",
  BIG_WIN = "voice/pd_table_bigwin",
  SMALL_WIN = "voice/pd_table_win",
  POK = "voice/pd_table_pok",
  NOTICE = "sounds/notice",
  CALL = "sounds/call",
  FOLD = "sounds/fold",
  RAISE = "sounds/raise",
  CHECK = "sounds/check",
  CLOCK_TICK = "sounds/clockTick",
  GEAR_TICK = "sounds/gearTick",
  FLIP_CARD = "sounds/flipCard",
  CHIP_DROP = "sounds/chipDropping",
  APPLAUSE = "sounds/Applause",
  SHOW_HAND_CARD = "sounds/ShowHandCard"
}
AudioManager.sound.magicEmojiSounds = {
  BOMB = "voice/pd_magic_bomb",
  DOG = "voice/pd_magic_dog",
  GHOST = "voice/pd_magic_ghost",
  KISS = "voice/pd_magic_kiss",
  ROSE = "voice/pd_magic_rose",
  WATER = "voice/pd_magic_water",
  TOMATO = "voice/pd_magic_tomato",
  COCK = "voice/pd_magic_cock",
  DAGGER = "voice/pd_magic_dagger",
  MISSILE = "voice/pd_magic_missile"
}
for k, v in pairs(AudioManager.sound.commonSounds) do
  AudioManager.sound[k] = v
end
for k, v in pairs(AudioManager.sound.tableSounds) do
  AudioManager.sound[k] = v
end
function AudioManager:ctor()
  if device.platform == "android" then
    self.format = ".ogg"
  else
    self.format = ".mp3"
  end
  for key, file in pairs(AudioManager.music) do
    AudioManager.music[key] = AudioManager.music[key] .. self.format
  end
  for key, file in pairs(AudioManager.sound) do
    if type(AudioManager.sound[key]) == "string" then
      AudioManager.sound[key] = AudioManager.sound[key] .. self.format
    end
  end
  for key, file in pairs(AudioManager.sound.commonSounds) do
    AudioManager.sound.commonSounds[key] = AudioManager.sound.commonSounds[key] .. self.format
  end
  for key, file in pairs(AudioManager.sound.tableSounds) do
    AudioManager.sound.tableSounds[key] = AudioManager.sound.tableSounds[key] .. self.format
  end
  for key, file in pairs(AudioManager.sound.magicEmojiSounds) do
    AudioManager.sound.magicEmojiSounds[key] = AudioManager.sound.magicEmojiSounds[key] .. self.format
  end
  self:initVolume()
end
function AudioManager:initVolume()
  self._volume = g.storage:getInt(g.vars.storageKeys.volume)
  if self._volume == nil then
    self._volume = 40
  end
  if g.storage:getBool(g.vars.storageKeys.volumeEnable, true) then
    audio.setSoundsVolume(self._volume / 100)
  else
    audio.setSoundsVolume(0)
  end
  audio.setMusicVolume(self._volume / 100)
end
function AudioManager:updateVolume()
  self._volume = g.storage:getInt(g.vars.storageKeys.volume)
  if g.storage:getBool(g.vars.storageKeys.volumeEnable, true) then
    audio.setSoundsVolume(self._volume / 100)
    audio.setMusicVolume(self._volume / 100)
  else
    audio.setSoundsVolume(0)
    audio.setMusicVolume(0)
  end
end
function AudioManager:preloadSound(soundType)
  if self.sound[soundType] and type(self.sound[soundType]) == "table" then
    for _, soundFile in pairs(self.sound[soundType]) do
      audio.preloadSound(soundFile)
    end
  end
end
function AudioManager:unloadSound(soundType)
  if self.sound[soundType] and type(self.sound[soundType]) == "table" then
    for _, soundFile in pairs(self.sound[soundType]) do
      audio.unloadSound(soundFile)
    end
  end
end
function AudioManager:playSound(soundKey, loop)
  if self._volume > 0 then
    local fileName = AudioManager.sound[soundKey]
    if fileName then
      return audio.playSound(fileName, loop or false)
    end
  end
  return nil
end
function AudioManager:stopAllSounds()
  audio.stopAllSounds()
end
function AudioManager:stopSound(soundHandler)
  if soundHandler then
    audio.stopSound(soundHandler)
  end
end
function AudioManager:playMagicEmojiSound(key)
  if self._volume > 0 then
    local fileName = AudioManager.sound.magicEmojiSounds[key]
    if fileName then
      audio.playSound(AudioManager.sound.magicEmojiSounds[key], false)
    end
  end
end
function AudioManager:preloadMusic()
  for music, file in pairs(self.music) do
    audio.preloadSound(file)
  end
end
function AudioManager:playMusic(musicType, isLoop)
  if self.music[musicType] then
    audio.playMusic(self.music[musicType], isLoop)
  end
end
function AudioManager:stopMusic(isReleaseData)
  audio.stopMusic(isReleaseData)
end
return AudioManager
