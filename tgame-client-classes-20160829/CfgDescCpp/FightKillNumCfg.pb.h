// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FightKillNumCfg.proto

#ifndef PROTOBUF_FightKillNumCfg_2eproto__INCLUDED
#define PROTOBUF_FightKillNumCfg_2eproto__INCLUDED

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
void  protobuf_AddDesc_FightKillNumCfg_2eproto();
void protobuf_AssignDesc_FightKillNumCfg_2eproto();
void protobuf_ShutdownFile_FightKillNumCfg_2eproto();

class FightKillNumCfg;
class FightKillNumCfgSet;

// ===================================================================

class FightKillNumCfg : public ::google::protobuf::MessageLite {
 public:
  FightKillNumCfg();
  virtual ~FightKillNumCfg();

  FightKillNumCfg(const FightKillNumCfg& from);

  inline FightKillNumCfg& operator=(const FightKillNumCfg& from) {
    CopyFrom(from);
    return *this;
  }

  static const FightKillNumCfg& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const FightKillNumCfg* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(FightKillNumCfg* other);

  // implements Message ----------------------------------------------

  FightKillNumCfg* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const FightKillNumCfg& from);
  void MergeFrom(const FightKillNumCfg& from);
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

  // required uint32 Lev = 1;
  inline bool has_lev() const;
  inline void clear_lev();
  static const int kLevFieldNumber = 1;
  inline ::google::protobuf::uint32 lev() const;
  inline void set_lev(::google::protobuf::uint32 value);

  // required uint32 KillNum = 2;
  inline bool has_killnum() const;
  inline void clear_killnum();
  static const int kKillNumFieldNumber = 2;
  inline ::google::protobuf::uint32 killnum() const;
  inline void set_killnum(::google::protobuf::uint32 value);

  // required string KillSound = 3;
  inline bool has_killsound() const;
  inline void clear_killsound();
  static const int kKillSoundFieldNumber = 3;
  inline const ::std::string& killsound() const;
  inline void set_killsound(const ::std::string& value);
  inline void set_killsound(const char* value);
  inline void set_killsound(const char* value, size_t size);
  inline ::std::string* mutable_killsound();
  inline ::std::string* release_killsound();
  inline void set_allocated_killsound(::std::string* killsound);

  // @@protoc_insertion_point(class_scope:com.cfg.vo.FightKillNumCfg)
 private:
  inline void set_has_lev();
  inline void clear_has_lev();
  inline void set_has_killnum();
  inline void clear_has_killnum();
  inline void set_has_killsound();
  inline void clear_has_killsound();

  ::google::protobuf::uint32 lev_;
  ::google::protobuf::uint32 killnum_;
  ::std::string* killsound_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_FightKillNumCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_FightKillNumCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_FightKillNumCfg_2eproto();
  friend void protobuf_ShutdownFile_FightKillNumCfg_2eproto();

  void InitAsDefaultInstance();
  static FightKillNumCfg* default_instance_;
};
// -------------------------------------------------------------------

class FightKillNumCfgSet : public ::google::protobuf::MessageLite {
 public:
  FightKillNumCfgSet();
  virtual ~FightKillNumCfgSet();

  FightKillNumCfgSet(const FightKillNumCfgSet& from);

  inline FightKillNumCfgSet& operator=(const FightKillNumCfgSet& from) {
    CopyFrom(from);
    return *this;
  }

  static const FightKillNumCfgSet& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const FightKillNumCfgSet* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(FightKillNumCfgSet* other);

  // implements Message ----------------------------------------------

  FightKillNumCfgSet* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const FightKillNumCfgSet& from);
  void MergeFrom(const FightKillNumCfgSet& from);
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

  // repeated .com.cfg.vo.FightKillNumCfg fightKillNumCfg = 1;
  inline int fightkillnumcfg_size() const;
  inline void clear_fightkillnumcfg();
  static const int kFightKillNumCfgFieldNumber = 1;
  inline const ::com::cfg::vo::FightKillNumCfg& fightkillnumcfg(int index) const;
  inline ::com::cfg::vo::FightKillNumCfg* mutable_fightkillnumcfg(int index);
  inline ::com::cfg::vo::FightKillNumCfg* add_fightkillnumcfg();
  inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::FightKillNumCfg >&
      fightkillnumcfg() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::FightKillNumCfg >*
      mutable_fightkillnumcfg();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.FightKillNumCfgSet)
 private:

  ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::FightKillNumCfg > fightkillnumcfg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_FightKillNumCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_FightKillNumCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_FightKillNumCfg_2eproto();
  friend void protobuf_ShutdownFile_FightKillNumCfg_2eproto();

  void InitAsDefaultInstance();
  static FightKillNumCfgSet* default_instance_;
};
// ===================================================================


// ===================================================================

// FightKillNumCfg

// required uint32 Lev = 1;
inline bool FightKillNumCfg::has_lev() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FightKillNumCfg::set_has_lev() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FightKillNumCfg::clear_has_lev() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FightKillNumCfg::clear_lev() {
  lev_ = 0u;
  clear_has_lev();
}
inline ::google::protobuf::uint32 FightKillNumCfg::lev() const {
  return lev_;
}
inline void FightKillNumCfg::set_lev(::google::protobuf::uint32 value) {
  set_has_lev();
  lev_ = value;
}

// required uint32 KillNum = 2;
inline bool FightKillNumCfg::has_killnum() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void FightKillNumCfg::set_has_killnum() {
  _has_bits_[0] |= 0x00000002u;
}
inline void FightKillNumCfg::clear_has_killnum() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void FightKillNumCfg::clear_killnum() {
  killnum_ = 0u;
  clear_has_killnum();
}
inline ::google::protobuf::uint32 FightKillNumCfg::killnum() const {
  return killnum_;
}
inline void FightKillNumCfg::set_killnum(::google::protobuf::uint32 value) {
  set_has_killnum();
  killnum_ = value;
}

// required string KillSound = 3;
inline bool FightKillNumCfg::has_killsound() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void FightKillNumCfg::set_has_killsound() {
  _has_bits_[0] |= 0x00000004u;
}
inline void FightKillNumCfg::clear_has_killsound() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void FightKillNumCfg::clear_killsound() {
  if (killsound_ != &::google::protobuf::internal::kEmptyString) {
    killsound_->clear();
  }
  clear_has_killsound();
}
inline const ::std::string& FightKillNumCfg::killsound() const {
  return *killsound_;
}
inline void FightKillNumCfg::set_killsound(const ::std::string& value) {
  set_has_killsound();
  if (killsound_ == &::google::protobuf::internal::kEmptyString) {
    killsound_ = new ::std::string;
  }
  killsound_->assign(value);
}
inline void FightKillNumCfg::set_killsound(const char* value) {
  set_has_killsound();
  if (killsound_ == &::google::protobuf::internal::kEmptyString) {
    killsound_ = new ::std::string;
  }
  killsound_->assign(value);
}
inline void FightKillNumCfg::set_killsound(const char* value, size_t size) {
  set_has_killsound();
  if (killsound_ == &::google::protobuf::internal::kEmptyString) {
    killsound_ = new ::std::string;
  }
  killsound_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FightKillNumCfg::mutable_killsound() {
  set_has_killsound();
  if (killsound_ == &::google::protobuf::internal::kEmptyString) {
    killsound_ = new ::std::string;
  }
  return killsound_;
}
inline ::std::string* FightKillNumCfg::release_killsound() {
  clear_has_killsound();
  if (killsound_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = killsound_;
    killsound_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void FightKillNumCfg::set_allocated_killsound(::std::string* killsound) {
  if (killsound_ != &::google::protobuf::internal::kEmptyString) {
    delete killsound_;
  }
  if (killsound) {
    set_has_killsound();
    killsound_ = killsound;
  } else {
    clear_has_killsound();
    killsound_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// FightKillNumCfgSet

// repeated .com.cfg.vo.FightKillNumCfg fightKillNumCfg = 1;
inline int FightKillNumCfgSet::fightkillnumcfg_size() const {
  return fightkillnumcfg_.size();
}
inline void FightKillNumCfgSet::clear_fightkillnumcfg() {
  fightkillnumcfg_.Clear();
}
inline const ::com::cfg::vo::FightKillNumCfg& FightKillNumCfgSet::fightkillnumcfg(int index) const {
  return fightkillnumcfg_.Get(index);
}
inline ::com::cfg::vo::FightKillNumCfg* FightKillNumCfgSet::mutable_fightkillnumcfg(int index) {
  return fightkillnumcfg_.Mutable(index);
}
inline ::com::cfg::vo::FightKillNumCfg* FightKillNumCfgSet::add_fightkillnumcfg() {
  return fightkillnumcfg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::FightKillNumCfg >&
FightKillNumCfgSet::fightkillnumcfg() const {
  return fightkillnumcfg_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::FightKillNumCfg >*
FightKillNumCfgSet::mutable_fightkillnumcfg() {
  return &fightkillnumcfg_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_FightKillNumCfg_2eproto__INCLUDED
