// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: OSDCfg.proto

#ifndef PROTOBUF_OSDCfg_2eproto__INCLUDED
#define PROTOBUF_OSDCfg_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace cfg {
namespace vo {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_OSDCfg_2eproto();
void protobuf_AssignDesc_OSDCfg_2eproto();
void protobuf_ShutdownFile_OSDCfg_2eproto();

class OSDCfg;
class OSDCfgSet;

// ===================================================================

class OSDCfg : public ::google::protobuf::MessageLite {
 public:
  OSDCfg();
  virtual ~OSDCfg();

  OSDCfg(const OSDCfg& from);

  inline OSDCfg& operator=(const OSDCfg& from) {
    CopyFrom(from);
    return *this;
  }

  static const OSDCfg& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const OSDCfg* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(OSDCfg* other);

  // implements Message ----------------------------------------------

  OSDCfg* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const OSDCfg& from);
  void MergeFrom(const OSDCfg& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint32 SceneID = 1;
  inline bool has_sceneid() const;
  inline void clear_sceneid();
  static const int kSceneIDFieldNumber = 1;
  inline ::google::protobuf::uint32 sceneid() const;
  inline void set_sceneid(::google::protobuf::uint32 value);

  // required string CfgID = 2;
  inline bool has_cfgid() const;
  inline void clear_cfgid();
  static const int kCfgIDFieldNumber = 2;
  inline const ::std::string& cfgid() const;
  inline void set_cfgid(const ::std::string& value);
  inline void set_cfgid(const char* value);
  inline void set_cfgid(const char* value, size_t size);
  inline ::std::string* mutable_cfgid();
  inline ::std::string* release_cfgid();
  inline void set_allocated_cfgid(::std::string* cfgid);

  // required uint32 TaskID = 3;
  inline bool has_taskid() const;
  inline void clear_taskid();
  static const int kTaskIDFieldNumber = 3;
  inline ::google::protobuf::uint32 taskid() const;
  inline void set_taskid(::google::protobuf::uint32 value);

  // repeated uint32 NPCLst = 4;
  inline int npclst_size() const;
  inline void clear_npclst();
  static const int kNPCLstFieldNumber = 4;
  inline ::google::protobuf::uint32 npclst(int index) const;
  inline void set_npclst(int index, ::google::protobuf::uint32 value);
  inline void add_npclst(::google::protobuf::uint32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      npclst() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_npclst();

  // required string FrontID = 6;
  inline bool has_frontid() const;
  inline void clear_frontid();
  static const int kFrontIDFieldNumber = 6;
  inline const ::std::string& frontid() const;
  inline void set_frontid(const ::std::string& value);
  inline void set_frontid(const char* value);
  inline void set_frontid(const char* value, size_t size);
  inline ::std::string* mutable_frontid();
  inline ::std::string* release_frontid();
  inline void set_allocated_frontid(::std::string* frontid);

  // required string ForeID = 7;
  inline bool has_foreid() const;
  inline void clear_foreid();
  static const int kForeIDFieldNumber = 7;
  inline const ::std::string& foreid() const;
  inline void set_foreid(const ::std::string& value);
  inline void set_foreid(const char* value);
  inline void set_foreid(const char* value, size_t size);
  inline ::std::string* mutable_foreid();
  inline ::std::string* release_foreid();
  inline void set_allocated_foreid(::std::string* foreid);

  // required string BackID = 8;
  inline bool has_backid() const;
  inline void clear_backid();
  static const int kBackIDFieldNumber = 8;
  inline const ::std::string& backid() const;
  inline void set_backid(const ::std::string& value);
  inline void set_backid(const char* value);
  inline void set_backid(const char* value, size_t size);
  inline ::std::string* mutable_backid();
  inline ::std::string* release_backid();
  inline void set_allocated_backid(::std::string* backid);

  // required uint32 WalkHeght = 9;
  inline bool has_walkheght() const;
  inline void clear_walkheght();
  static const int kWalkHeghtFieldNumber = 9;
  inline ::google::protobuf::uint32 walkheght() const;
  inline void set_walkheght(::google::protobuf::uint32 value);

  // required uint32 SceneEffectID = 10;
  inline bool has_sceneeffectid() const;
  inline void clear_sceneeffectid();
  static const int kSceneEffectIDFieldNumber = 10;
  inline ::google::protobuf::uint32 sceneeffectid() const;
  inline void set_sceneeffectid(::google::protobuf::uint32 value);

  // required string MidID = 11;
  inline bool has_midid() const;
  inline void clear_midid();
  static const int kMidIDFieldNumber = 11;
  inline const ::std::string& midid() const;
  inline void set_midid(const ::std::string& value);
  inline void set_midid(const char* value);
  inline void set_midid(const char* value, size_t size);
  inline ::std::string* mutable_midid();
  inline ::std::string* release_midid();
  inline void set_allocated_midid(::std::string* midid);

  // required string MidID2 = 12;
  inline bool has_midid2() const;
  inline void clear_midid2();
  static const int kMidID2FieldNumber = 12;
  inline const ::std::string& midid2() const;
  inline void set_midid2(const ::std::string& value);
  inline void set_midid2(const char* value);
  inline void set_midid2(const char* value, size_t size);
  inline ::std::string* mutable_midid2();
  inline ::std::string* release_midid2();
  inline void set_allocated_midid2(::std::string* midid2);

  // required uint32 MidPos = 13;
  inline bool has_midpos() const;
  inline void clear_midpos();
  static const int kMidPosFieldNumber = 13;
  inline ::google::protobuf::uint32 midpos() const;
  inline void set_midpos(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:com.cfg.vo.OSDCfg)
 private:
  inline void set_has_sceneid();
  inline void clear_has_sceneid();
  inline void set_has_cfgid();
  inline void clear_has_cfgid();
  inline void set_has_taskid();
  inline void clear_has_taskid();
  inline void set_has_frontid();
  inline void clear_has_frontid();
  inline void set_has_foreid();
  inline void clear_has_foreid();
  inline void set_has_backid();
  inline void clear_has_backid();
  inline void set_has_walkheght();
  inline void clear_has_walkheght();
  inline void set_has_sceneeffectid();
  inline void clear_has_sceneeffectid();
  inline void set_has_midid();
  inline void clear_has_midid();
  inline void set_has_midid2();
  inline void clear_has_midid2();
  inline void set_has_midpos();
  inline void clear_has_midpos();

  ::std::string* cfgid_;
  ::google::protobuf::uint32 sceneid_;
  ::google::protobuf::uint32 taskid_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > npclst_;
  ::std::string* frontid_;
  ::std::string* foreid_;
  ::std::string* backid_;
  ::google::protobuf::uint32 walkheght_;
  ::google::protobuf::uint32 sceneeffectid_;
  ::std::string* midid_;
  ::std::string* midid2_;
  ::google::protobuf::uint32 midpos_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(12 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_OSDCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_OSDCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_OSDCfg_2eproto();
  friend void protobuf_ShutdownFile_OSDCfg_2eproto();

  void InitAsDefaultInstance();
  static OSDCfg* default_instance_;
};
// -------------------------------------------------------------------

class OSDCfgSet : public ::google::protobuf::MessageLite {
 public:
  OSDCfgSet();
  virtual ~OSDCfgSet();

  OSDCfgSet(const OSDCfgSet& from);

  inline OSDCfgSet& operator=(const OSDCfgSet& from) {
    CopyFrom(from);
    return *this;
  }

  static const OSDCfgSet& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const OSDCfgSet* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(OSDCfgSet* other);

  // implements Message ----------------------------------------------

  OSDCfgSet* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const OSDCfgSet& from);
  void MergeFrom(const OSDCfgSet& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .com.cfg.vo.OSDCfg osdcfg = 1;
  inline int osdcfg_size() const;
  inline void clear_osdcfg();
  static const int kOsdcfgFieldNumber = 1;
  inline const ::com::cfg::vo::OSDCfg& osdcfg(int index) const;
  inline ::com::cfg::vo::OSDCfg* mutable_osdcfg(int index);
  inline ::com::cfg::vo::OSDCfg* add_osdcfg();
  inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::OSDCfg >&
      osdcfg() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::OSDCfg >*
      mutable_osdcfg();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.OSDCfgSet)
 private:

  ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::OSDCfg > osdcfg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_OSDCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_OSDCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_OSDCfg_2eproto();
  friend void protobuf_ShutdownFile_OSDCfg_2eproto();

  void InitAsDefaultInstance();
  static OSDCfgSet* default_instance_;
};
// ===================================================================


// ===================================================================

// OSDCfg

// required uint32 SceneID = 1;
inline bool OSDCfg::has_sceneid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void OSDCfg::set_has_sceneid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void OSDCfg::clear_has_sceneid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void OSDCfg::clear_sceneid() {
  sceneid_ = 0u;
  clear_has_sceneid();
}
inline ::google::protobuf::uint32 OSDCfg::sceneid() const {
  return sceneid_;
}
inline void OSDCfg::set_sceneid(::google::protobuf::uint32 value) {
  set_has_sceneid();
  sceneid_ = value;
}

// required string CfgID = 2;
inline bool OSDCfg::has_cfgid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void OSDCfg::set_has_cfgid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void OSDCfg::clear_has_cfgid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void OSDCfg::clear_cfgid() {
  if (cfgid_ != &::google::protobuf::internal::kEmptyString) {
    cfgid_->clear();
  }
  clear_has_cfgid();
}
inline const ::std::string& OSDCfg::cfgid() const {
  return *cfgid_;
}
inline void OSDCfg::set_cfgid(const ::std::string& value) {
  set_has_cfgid();
  if (cfgid_ == &::google::protobuf::internal::kEmptyString) {
    cfgid_ = new ::std::string;
  }
  cfgid_->assign(value);
}
inline void OSDCfg::set_cfgid(const char* value) {
  set_has_cfgid();
  if (cfgid_ == &::google::protobuf::internal::kEmptyString) {
    cfgid_ = new ::std::string;
  }
  cfgid_->assign(value);
}
inline void OSDCfg::set_cfgid(const char* value, size_t size) {
  set_has_cfgid();
  if (cfgid_ == &::google::protobuf::internal::kEmptyString) {
    cfgid_ = new ::std::string;
  }
  cfgid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* OSDCfg::mutable_cfgid() {
  set_has_cfgid();
  if (cfgid_ == &::google::protobuf::internal::kEmptyString) {
    cfgid_ = new ::std::string;
  }
  return cfgid_;
}
inline ::std::string* OSDCfg::release_cfgid() {
  clear_has_cfgid();
  if (cfgid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = cfgid_;
    cfgid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void OSDCfg::set_allocated_cfgid(::std::string* cfgid) {
  if (cfgid_ != &::google::protobuf::internal::kEmptyString) {
    delete cfgid_;
  }
  if (cfgid) {
    set_has_cfgid();
    cfgid_ = cfgid;
  } else {
    clear_has_cfgid();
    cfgid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 TaskID = 3;
inline bool OSDCfg::has_taskid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void OSDCfg::set_has_taskid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void OSDCfg::clear_has_taskid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void OSDCfg::clear_taskid() {
  taskid_ = 0u;
  clear_has_taskid();
}
inline ::google::protobuf::uint32 OSDCfg::taskid() const {
  return taskid_;
}
inline void OSDCfg::set_taskid(::google::protobuf::uint32 value) {
  set_has_taskid();
  taskid_ = value;
}

// repeated uint32 NPCLst = 4;
inline int OSDCfg::npclst_size() const {
  return npclst_.size();
}
inline void OSDCfg::clear_npclst() {
  npclst_.Clear();
}
inline ::google::protobuf::uint32 OSDCfg::npclst(int index) const {
  return npclst_.Get(index);
}
inline void OSDCfg::set_npclst(int index, ::google::protobuf::uint32 value) {
  npclst_.Set(index, value);
}
inline void OSDCfg::add_npclst(::google::protobuf::uint32 value) {
  npclst_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
OSDCfg::npclst() const {
  return npclst_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
OSDCfg::mutable_npclst() {
  return &npclst_;
}

// required string FrontID = 6;
inline bool OSDCfg::has_frontid() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void OSDCfg::set_has_frontid() {
  _has_bits_[0] |= 0x00000010u;
}
inline void OSDCfg::clear_has_frontid() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void OSDCfg::clear_frontid() {
  if (frontid_ != &::google::protobuf::internal::kEmptyString) {
    frontid_->clear();
  }
  clear_has_frontid();
}
inline const ::std::string& OSDCfg::frontid() const {
  return *frontid_;
}
inline void OSDCfg::set_frontid(const ::std::string& value) {
  set_has_frontid();
  if (frontid_ == &::google::protobuf::internal::kEmptyString) {
    frontid_ = new ::std::string;
  }
  frontid_->assign(value);
}
inline void OSDCfg::set_frontid(const char* value) {
  set_has_frontid();
  if (frontid_ == &::google::protobuf::internal::kEmptyString) {
    frontid_ = new ::std::string;
  }
  frontid_->assign(value);
}
inline void OSDCfg::set_frontid(const char* value, size_t size) {
  set_has_frontid();
  if (frontid_ == &::google::protobuf::internal::kEmptyString) {
    frontid_ = new ::std::string;
  }
  frontid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* OSDCfg::mutable_frontid() {
  set_has_frontid();
  if (frontid_ == &::google::protobuf::internal::kEmptyString) {
    frontid_ = new ::std::string;
  }
  return frontid_;
}
inline ::std::string* OSDCfg::release_frontid() {
  clear_has_frontid();
  if (frontid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = frontid_;
    frontid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void OSDCfg::set_allocated_frontid(::std::string* frontid) {
  if (frontid_ != &::google::protobuf::internal::kEmptyString) {
    delete frontid_;
  }
  if (frontid) {
    set_has_frontid();
    frontid_ = frontid;
  } else {
    clear_has_frontid();
    frontid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string ForeID = 7;
inline bool OSDCfg::has_foreid() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void OSDCfg::set_has_foreid() {
  _has_bits_[0] |= 0x00000020u;
}
inline void OSDCfg::clear_has_foreid() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void OSDCfg::clear_foreid() {
  if (foreid_ != &::google::protobuf::internal::kEmptyString) {
    foreid_->clear();
  }
  clear_has_foreid();
}
inline const ::std::string& OSDCfg::foreid() const {
  return *foreid_;
}
inline void OSDCfg::set_foreid(const ::std::string& value) {
  set_has_foreid();
  if (foreid_ == &::google::protobuf::internal::kEmptyString) {
    foreid_ = new ::std::string;
  }
  foreid_->assign(value);
}
inline void OSDCfg::set_foreid(const char* value) {
  set_has_foreid();
  if (foreid_ == &::google::protobuf::internal::kEmptyString) {
    foreid_ = new ::std::string;
  }
  foreid_->assign(value);
}
inline void OSDCfg::set_foreid(const char* value, size_t size) {
  set_has_foreid();
  if (foreid_ == &::google::protobuf::internal::kEmptyString) {
    foreid_ = new ::std::string;
  }
  foreid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* OSDCfg::mutable_foreid() {
  set_has_foreid();
  if (foreid_ == &::google::protobuf::internal::kEmptyString) {
    foreid_ = new ::std::string;
  }
  return foreid_;
}
inline ::std::string* OSDCfg::release_foreid() {
  clear_has_foreid();
  if (foreid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = foreid_;
    foreid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void OSDCfg::set_allocated_foreid(::std::string* foreid) {
  if (foreid_ != &::google::protobuf::internal::kEmptyString) {
    delete foreid_;
  }
  if (foreid) {
    set_has_foreid();
    foreid_ = foreid;
  } else {
    clear_has_foreid();
    foreid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string BackID = 8;
inline bool OSDCfg::has_backid() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void OSDCfg::set_has_backid() {
  _has_bits_[0] |= 0x00000040u;
}
inline void OSDCfg::clear_has_backid() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void OSDCfg::clear_backid() {
  if (backid_ != &::google::protobuf::internal::kEmptyString) {
    backid_->clear();
  }
  clear_has_backid();
}
inline const ::std::string& OSDCfg::backid() const {
  return *backid_;
}
inline void OSDCfg::set_backid(const ::std::string& value) {
  set_has_backid();
  if (backid_ == &::google::protobuf::internal::kEmptyString) {
    backid_ = new ::std::string;
  }
  backid_->assign(value);
}
inline void OSDCfg::set_backid(const char* value) {
  set_has_backid();
  if (backid_ == &::google::protobuf::internal::kEmptyString) {
    backid_ = new ::std::string;
  }
  backid_->assign(value);
}
inline void OSDCfg::set_backid(const char* value, size_t size) {
  set_has_backid();
  if (backid_ == &::google::protobuf::internal::kEmptyString) {
    backid_ = new ::std::string;
  }
  backid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* OSDCfg::mutable_backid() {
  set_has_backid();
  if (backid_ == &::google::protobuf::internal::kEmptyString) {
    backid_ = new ::std::string;
  }
  return backid_;
}
inline ::std::string* OSDCfg::release_backid() {
  clear_has_backid();
  if (backid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = backid_;
    backid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void OSDCfg::set_allocated_backid(::std::string* backid) {
  if (backid_ != &::google::protobuf::internal::kEmptyString) {
    delete backid_;
  }
  if (backid) {
    set_has_backid();
    backid_ = backid;
  } else {
    clear_has_backid();
    backid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 WalkHeght = 9;
inline bool OSDCfg::has_walkheght() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void OSDCfg::set_has_walkheght() {
  _has_bits_[0] |= 0x00000080u;
}
inline void OSDCfg::clear_has_walkheght() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void OSDCfg::clear_walkheght() {
  walkheght_ = 0u;
  clear_has_walkheght();
}
inline ::google::protobuf::uint32 OSDCfg::walkheght() const {
  return walkheght_;
}
inline void OSDCfg::set_walkheght(::google::protobuf::uint32 value) {
  set_has_walkheght();
  walkheght_ = value;
}

// required uint32 SceneEffectID = 10;
inline bool OSDCfg::has_sceneeffectid() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void OSDCfg::set_has_sceneeffectid() {
  _has_bits_[0] |= 0x00000100u;
}
inline void OSDCfg::clear_has_sceneeffectid() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void OSDCfg::clear_sceneeffectid() {
  sceneeffectid_ = 0u;
  clear_has_sceneeffectid();
}
inline ::google::protobuf::uint32 OSDCfg::sceneeffectid() const {
  return sceneeffectid_;
}
inline void OSDCfg::set_sceneeffectid(::google::protobuf::uint32 value) {
  set_has_sceneeffectid();
  sceneeffectid_ = value;
}

// required string MidID = 11;
inline bool OSDCfg::has_midid() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void OSDCfg::set_has_midid() {
  _has_bits_[0] |= 0x00000200u;
}
inline void OSDCfg::clear_has_midid() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void OSDCfg::clear_midid() {
  if (midid_ != &::google::protobuf::internal::kEmptyString) {
    midid_->clear();
  }
  clear_has_midid();
}
inline const ::std::string& OSDCfg::midid() const {
  return *midid_;
}
inline void OSDCfg::set_midid(const ::std::string& value) {
  set_has_midid();
  if (midid_ == &::google::protobuf::internal::kEmptyString) {
    midid_ = new ::std::string;
  }
  midid_->assign(value);
}
inline void OSDCfg::set_midid(const char* value) {
  set_has_midid();
  if (midid_ == &::google::protobuf::internal::kEmptyString) {
    midid_ = new ::std::string;
  }
  midid_->assign(value);
}
inline void OSDCfg::set_midid(const char* value, size_t size) {
  set_has_midid();
  if (midid_ == &::google::protobuf::internal::kEmptyString) {
    midid_ = new ::std::string;
  }
  midid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* OSDCfg::mutable_midid() {
  set_has_midid();
  if (midid_ == &::google::protobuf::internal::kEmptyString) {
    midid_ = new ::std::string;
  }
  return midid_;
}
inline ::std::string* OSDCfg::release_midid() {
  clear_has_midid();
  if (midid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = midid_;
    midid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void OSDCfg::set_allocated_midid(::std::string* midid) {
  if (midid_ != &::google::protobuf::internal::kEmptyString) {
    delete midid_;
  }
  if (midid) {
    set_has_midid();
    midid_ = midid;
  } else {
    clear_has_midid();
    midid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string MidID2 = 12;
inline bool OSDCfg::has_midid2() const {
  return (_has_bits_[0] & 0x00000400u) != 0;
}
inline void OSDCfg::set_has_midid2() {
  _has_bits_[0] |= 0x00000400u;
}
inline void OSDCfg::clear_has_midid2() {
  _has_bits_[0] &= ~0x00000400u;
}
inline void OSDCfg::clear_midid2() {
  if (midid2_ != &::google::protobuf::internal::kEmptyString) {
    midid2_->clear();
  }
  clear_has_midid2();
}
inline const ::std::string& OSDCfg::midid2() const {
  return *midid2_;
}
inline void OSDCfg::set_midid2(const ::std::string& value) {
  set_has_midid2();
  if (midid2_ == &::google::protobuf::internal::kEmptyString) {
    midid2_ = new ::std::string;
  }
  midid2_->assign(value);
}
inline void OSDCfg::set_midid2(const char* value) {
  set_has_midid2();
  if (midid2_ == &::google::protobuf::internal::kEmptyString) {
    midid2_ = new ::std::string;
  }
  midid2_->assign(value);
}
inline void OSDCfg::set_midid2(const char* value, size_t size) {
  set_has_midid2();
  if (midid2_ == &::google::protobuf::internal::kEmptyString) {
    midid2_ = new ::std::string;
  }
  midid2_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* OSDCfg::mutable_midid2() {
  set_has_midid2();
  if (midid2_ == &::google::protobuf::internal::kEmptyString) {
    midid2_ = new ::std::string;
  }
  return midid2_;
}
inline ::std::string* OSDCfg::release_midid2() {
  clear_has_midid2();
  if (midid2_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = midid2_;
    midid2_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void OSDCfg::set_allocated_midid2(::std::string* midid2) {
  if (midid2_ != &::google::protobuf::internal::kEmptyString) {
    delete midid2_;
  }
  if (midid2) {
    set_has_midid2();
    midid2_ = midid2;
  } else {
    clear_has_midid2();
    midid2_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required uint32 MidPos = 13;
inline bool OSDCfg::has_midpos() const {
  return (_has_bits_[0] & 0x00000800u) != 0;
}
inline void OSDCfg::set_has_midpos() {
  _has_bits_[0] |= 0x00000800u;
}
inline void OSDCfg::clear_has_midpos() {
  _has_bits_[0] &= ~0x00000800u;
}
inline void OSDCfg::clear_midpos() {
  midpos_ = 0u;
  clear_has_midpos();
}
inline ::google::protobuf::uint32 OSDCfg::midpos() const {
  return midpos_;
}
inline void OSDCfg::set_midpos(::google::protobuf::uint32 value) {
  set_has_midpos();
  midpos_ = value;
}

// -------------------------------------------------------------------

// OSDCfgSet

// repeated .com.cfg.vo.OSDCfg osdcfg = 1;
inline int OSDCfgSet::osdcfg_size() const {
  return osdcfg_.size();
}
inline void OSDCfgSet::clear_osdcfg() {
  osdcfg_.Clear();
}
inline const ::com::cfg::vo::OSDCfg& OSDCfgSet::osdcfg(int index) const {
  return osdcfg_.Get(index);
}
inline ::com::cfg::vo::OSDCfg* OSDCfgSet::mutable_osdcfg(int index) {
  return osdcfg_.Mutable(index);
}
inline ::com::cfg::vo::OSDCfg* OSDCfgSet::add_osdcfg() {
  return osdcfg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::OSDCfg >&
OSDCfgSet::osdcfg() const {
  return osdcfg_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::OSDCfg >*
OSDCfgSet::mutable_osdcfg() {
  return &osdcfg_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_OSDCfg_2eproto__INCLUDED