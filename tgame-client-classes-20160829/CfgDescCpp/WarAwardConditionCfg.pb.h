// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WarAwardConditionCfg.proto

#ifndef PROTOBUF_WarAwardConditionCfg_2eproto__INCLUDED
#define PROTOBUF_WarAwardConditionCfg_2eproto__INCLUDED

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
void  protobuf_AddDesc_WarAwardConditionCfg_2eproto();
void protobuf_AssignDesc_WarAwardConditionCfg_2eproto();
void protobuf_ShutdownFile_WarAwardConditionCfg_2eproto();

class WarAwardConditionCfg;
class WarAwardConditionCfgSet;

// ===================================================================

class WarAwardConditionCfg : public ::google::protobuf::MessageLite {
 public:
  WarAwardConditionCfg();
  virtual ~WarAwardConditionCfg();

  WarAwardConditionCfg(const WarAwardConditionCfg& from);

  inline WarAwardConditionCfg& operator=(const WarAwardConditionCfg& from) {
    CopyFrom(from);
    return *this;
  }

  static const WarAwardConditionCfg& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const WarAwardConditionCfg* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(WarAwardConditionCfg* other);

  // implements Message ----------------------------------------------

  WarAwardConditionCfg* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const WarAwardConditionCfg& from);
  void MergeFrom(const WarAwardConditionCfg& from);
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

  // required uint32 Type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::google::protobuf::uint32 type() const;
  inline void set_type(::google::protobuf::uint32 value);

  // required string Desc = 2;
  inline bool has_desc() const;
  inline void clear_desc();
  static const int kDescFieldNumber = 2;
  inline const ::std::string& desc() const;
  inline void set_desc(const ::std::string& value);
  inline void set_desc(const char* value);
  inline void set_desc(const char* value, size_t size);
  inline ::std::string* mutable_desc();
  inline ::std::string* release_desc();
  inline void set_allocated_desc(::std::string* desc);

  // @@protoc_insertion_point(class_scope:com.cfg.vo.WarAwardConditionCfg)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_desc();
  inline void clear_has_desc();

  ::std::string* desc_;
  ::google::protobuf::uint32 type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_WarAwardConditionCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_WarAwardConditionCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_WarAwardConditionCfg_2eproto();
  friend void protobuf_ShutdownFile_WarAwardConditionCfg_2eproto();

  void InitAsDefaultInstance();
  static WarAwardConditionCfg* default_instance_;
};
// -------------------------------------------------------------------

class WarAwardConditionCfgSet : public ::google::protobuf::MessageLite {
 public:
  WarAwardConditionCfgSet();
  virtual ~WarAwardConditionCfgSet();

  WarAwardConditionCfgSet(const WarAwardConditionCfgSet& from);

  inline WarAwardConditionCfgSet& operator=(const WarAwardConditionCfgSet& from) {
    CopyFrom(from);
    return *this;
  }

  static const WarAwardConditionCfgSet& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const WarAwardConditionCfgSet* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(WarAwardConditionCfgSet* other);

  // implements Message ----------------------------------------------

  WarAwardConditionCfgSet* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const WarAwardConditionCfgSet& from);
  void MergeFrom(const WarAwardConditionCfgSet& from);
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

  // repeated .com.cfg.vo.WarAwardConditionCfg warAwardConditionCfg = 1;
  inline int warawardconditioncfg_size() const;
  inline void clear_warawardconditioncfg();
  static const int kWarAwardConditionCfgFieldNumber = 1;
  inline const ::com::cfg::vo::WarAwardConditionCfg& warawardconditioncfg(int index) const;
  inline ::com::cfg::vo::WarAwardConditionCfg* mutable_warawardconditioncfg(int index);
  inline ::com::cfg::vo::WarAwardConditionCfg* add_warawardconditioncfg();
  inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::WarAwardConditionCfg >&
      warawardconditioncfg() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::WarAwardConditionCfg >*
      mutable_warawardconditioncfg();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.WarAwardConditionCfgSet)
 private:

  ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::WarAwardConditionCfg > warawardconditioncfg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_WarAwardConditionCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_WarAwardConditionCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_WarAwardConditionCfg_2eproto();
  friend void protobuf_ShutdownFile_WarAwardConditionCfg_2eproto();

  void InitAsDefaultInstance();
  static WarAwardConditionCfgSet* default_instance_;
};
// ===================================================================


// ===================================================================

// WarAwardConditionCfg

// required uint32 Type = 1;
inline bool WarAwardConditionCfg::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void WarAwardConditionCfg::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void WarAwardConditionCfg::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void WarAwardConditionCfg::clear_type() {
  type_ = 0u;
  clear_has_type();
}
inline ::google::protobuf::uint32 WarAwardConditionCfg::type() const {
  return type_;
}
inline void WarAwardConditionCfg::set_type(::google::protobuf::uint32 value) {
  set_has_type();
  type_ = value;
}

// required string Desc = 2;
inline bool WarAwardConditionCfg::has_desc() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void WarAwardConditionCfg::set_has_desc() {
  _has_bits_[0] |= 0x00000002u;
}
inline void WarAwardConditionCfg::clear_has_desc() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void WarAwardConditionCfg::clear_desc() {
  if (desc_ != &::google::protobuf::internal::kEmptyString) {
    desc_->clear();
  }
  clear_has_desc();
}
inline const ::std::string& WarAwardConditionCfg::desc() const {
  return *desc_;
}
inline void WarAwardConditionCfg::set_desc(const ::std::string& value) {
  set_has_desc();
  if (desc_ == &::google::protobuf::internal::kEmptyString) {
    desc_ = new ::std::string;
  }
  desc_->assign(value);
}
inline void WarAwardConditionCfg::set_desc(const char* value) {
  set_has_desc();
  if (desc_ == &::google::protobuf::internal::kEmptyString) {
    desc_ = new ::std::string;
  }
  desc_->assign(value);
}
inline void WarAwardConditionCfg::set_desc(const char* value, size_t size) {
  set_has_desc();
  if (desc_ == &::google::protobuf::internal::kEmptyString) {
    desc_ = new ::std::string;
  }
  desc_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* WarAwardConditionCfg::mutable_desc() {
  set_has_desc();
  if (desc_ == &::google::protobuf::internal::kEmptyString) {
    desc_ = new ::std::string;
  }
  return desc_;
}
inline ::std::string* WarAwardConditionCfg::release_desc() {
  clear_has_desc();
  if (desc_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = desc_;
    desc_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void WarAwardConditionCfg::set_allocated_desc(::std::string* desc) {
  if (desc_ != &::google::protobuf::internal::kEmptyString) {
    delete desc_;
  }
  if (desc) {
    set_has_desc();
    desc_ = desc;
  } else {
    clear_has_desc();
    desc_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// WarAwardConditionCfgSet

// repeated .com.cfg.vo.WarAwardConditionCfg warAwardConditionCfg = 1;
inline int WarAwardConditionCfgSet::warawardconditioncfg_size() const {
  return warawardconditioncfg_.size();
}
inline void WarAwardConditionCfgSet::clear_warawardconditioncfg() {
  warawardconditioncfg_.Clear();
}
inline const ::com::cfg::vo::WarAwardConditionCfg& WarAwardConditionCfgSet::warawardconditioncfg(int index) const {
  return warawardconditioncfg_.Get(index);
}
inline ::com::cfg::vo::WarAwardConditionCfg* WarAwardConditionCfgSet::mutable_warawardconditioncfg(int index) {
  return warawardconditioncfg_.Mutable(index);
}
inline ::com::cfg::vo::WarAwardConditionCfg* WarAwardConditionCfgSet::add_warawardconditioncfg() {
  return warawardconditioncfg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::WarAwardConditionCfg >&
WarAwardConditionCfgSet::warawardconditioncfg() const {
  return warawardconditioncfg_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::WarAwardConditionCfg >*
WarAwardConditionCfgSet::mutable_warawardconditioncfg() {
  return &warawardconditioncfg_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_WarAwardConditionCfg_2eproto__INCLUDED