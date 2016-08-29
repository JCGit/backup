// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DropCfg.proto

#ifndef PROTOBUF_DropCfg_2eproto__INCLUDED
#define PROTOBUF_DropCfg_2eproto__INCLUDED

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
void  protobuf_AddDesc_DropCfg_2eproto();
void protobuf_AssignDesc_DropCfg_2eproto();
void protobuf_ShutdownFile_DropCfg_2eproto();

class DropElement;
class DropCfg;
class DropCfgSet;

// ===================================================================

class DropElement : public ::google::protobuf::MessageLite {
 public:
  DropElement();
  virtual ~DropElement();

  DropElement(const DropElement& from);

  inline DropElement& operator=(const DropElement& from) {
    CopyFrom(from);
    return *this;
  }

  static const DropElement& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const DropElement* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(DropElement* other);

  // implements Message ----------------------------------------------

  DropElement* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const DropElement& from);
  void MergeFrom(const DropElement& from);
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

  // required uint32 ElementID = 1;
  inline bool has_elementid() const;
  inline void clear_elementid();
  static const int kElementIDFieldNumber = 1;
  inline ::google::protobuf::uint32 elementid() const;
  inline void set_elementid(::google::protobuf::uint32 value);

  // required uint32 ElementType = 2;
  inline bool has_elementtype() const;
  inline void clear_elementtype();
  static const int kElementTypeFieldNumber = 2;
  inline ::google::protobuf::uint32 elementtype() const;
  inline void set_elementtype(::google::protobuf::uint32 value);

  // required uint32 DropProb = 5;
  inline bool has_dropprob() const;
  inline void clear_dropprob();
  static const int kDropProbFieldNumber = 5;
  inline ::google::protobuf::uint32 dropprob() const;
  inline void set_dropprob(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:com.cfg.vo.DropElement)
 private:
  inline void set_has_elementid();
  inline void clear_has_elementid();
  inline void set_has_elementtype();
  inline void clear_has_elementtype();
  inline void set_has_dropprob();
  inline void clear_has_dropprob();

  ::google::protobuf::uint32 elementid_;
  ::google::protobuf::uint32 elementtype_;
  ::google::protobuf::uint32 dropprob_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_DropCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_DropCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_DropCfg_2eproto();
  friend void protobuf_ShutdownFile_DropCfg_2eproto();

  void InitAsDefaultInstance();
  static DropElement* default_instance_;
};
// -------------------------------------------------------------------

class DropCfg : public ::google::protobuf::MessageLite {
 public:
  DropCfg();
  virtual ~DropCfg();

  DropCfg(const DropCfg& from);

  inline DropCfg& operator=(const DropCfg& from) {
    CopyFrom(from);
    return *this;
  }

  static const DropCfg& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const DropCfg* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(DropCfg* other);

  // implements Message ----------------------------------------------

  DropCfg* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const DropCfg& from);
  void MergeFrom(const DropCfg& from);
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

  // required uint32 DropID = 1;
  inline bool has_dropid() const;
  inline void clear_dropid();
  static const int kDropIDFieldNumber = 1;
  inline ::google::protobuf::uint32 dropid() const;
  inline void set_dropid(::google::protobuf::uint32 value);

  // required .com.cfg.vo.DropElement ElementLst = 5;
  inline bool has_elementlst() const;
  inline void clear_elementlst();
  static const int kElementLstFieldNumber = 5;
  inline const ::com::cfg::vo::DropElement& elementlst() const;
  inline ::com::cfg::vo::DropElement* mutable_elementlst();
  inline ::com::cfg::vo::DropElement* release_elementlst();
  inline void set_allocated_elementlst(::com::cfg::vo::DropElement* elementlst);

  // @@protoc_insertion_point(class_scope:com.cfg.vo.DropCfg)
 private:
  inline void set_has_dropid();
  inline void clear_has_dropid();
  inline void set_has_elementlst();
  inline void clear_has_elementlst();

  ::com::cfg::vo::DropElement* elementlst_;
  ::google::protobuf::uint32 dropid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_DropCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_DropCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_DropCfg_2eproto();
  friend void protobuf_ShutdownFile_DropCfg_2eproto();

  void InitAsDefaultInstance();
  static DropCfg* default_instance_;
};
// -------------------------------------------------------------------

class DropCfgSet : public ::google::protobuf::MessageLite {
 public:
  DropCfgSet();
  virtual ~DropCfgSet();

  DropCfgSet(const DropCfgSet& from);

  inline DropCfgSet& operator=(const DropCfgSet& from) {
    CopyFrom(from);
    return *this;
  }

  static const DropCfgSet& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const DropCfgSet* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(DropCfgSet* other);

  // implements Message ----------------------------------------------

  DropCfgSet* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const DropCfgSet& from);
  void MergeFrom(const DropCfgSet& from);
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

  // repeated .com.cfg.vo.DropCfg dropcfg = 1;
  inline int dropcfg_size() const;
  inline void clear_dropcfg();
  static const int kDropcfgFieldNumber = 1;
  inline const ::com::cfg::vo::DropCfg& dropcfg(int index) const;
  inline ::com::cfg::vo::DropCfg* mutable_dropcfg(int index);
  inline ::com::cfg::vo::DropCfg* add_dropcfg();
  inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::DropCfg >&
      dropcfg() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::DropCfg >*
      mutable_dropcfg();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.DropCfgSet)
 private:

  ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::DropCfg > dropcfg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_DropCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_DropCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_DropCfg_2eproto();
  friend void protobuf_ShutdownFile_DropCfg_2eproto();

  void InitAsDefaultInstance();
  static DropCfgSet* default_instance_;
};
// ===================================================================


// ===================================================================

// DropElement

// required uint32 ElementID = 1;
inline bool DropElement::has_elementid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DropElement::set_has_elementid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DropElement::clear_has_elementid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DropElement::clear_elementid() {
  elementid_ = 0u;
  clear_has_elementid();
}
inline ::google::protobuf::uint32 DropElement::elementid() const {
  return elementid_;
}
inline void DropElement::set_elementid(::google::protobuf::uint32 value) {
  set_has_elementid();
  elementid_ = value;
}

// required uint32 ElementType = 2;
inline bool DropElement::has_elementtype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DropElement::set_has_elementtype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DropElement::clear_has_elementtype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DropElement::clear_elementtype() {
  elementtype_ = 0u;
  clear_has_elementtype();
}
inline ::google::protobuf::uint32 DropElement::elementtype() const {
  return elementtype_;
}
inline void DropElement::set_elementtype(::google::protobuf::uint32 value) {
  set_has_elementtype();
  elementtype_ = value;
}

// required uint32 DropProb = 5;
inline bool DropElement::has_dropprob() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void DropElement::set_has_dropprob() {
  _has_bits_[0] |= 0x00000004u;
}
inline void DropElement::clear_has_dropprob() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void DropElement::clear_dropprob() {
  dropprob_ = 0u;
  clear_has_dropprob();
}
inline ::google::protobuf::uint32 DropElement::dropprob() const {
  return dropprob_;
}
inline void DropElement::set_dropprob(::google::protobuf::uint32 value) {
  set_has_dropprob();
  dropprob_ = value;
}

// -------------------------------------------------------------------

// DropCfg

// required uint32 DropID = 1;
inline bool DropCfg::has_dropid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DropCfg::set_has_dropid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DropCfg::clear_has_dropid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DropCfg::clear_dropid() {
  dropid_ = 0u;
  clear_has_dropid();
}
inline ::google::protobuf::uint32 DropCfg::dropid() const {
  return dropid_;
}
inline void DropCfg::set_dropid(::google::protobuf::uint32 value) {
  set_has_dropid();
  dropid_ = value;
}

// required .com.cfg.vo.DropElement ElementLst = 5;
inline bool DropCfg::has_elementlst() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DropCfg::set_has_elementlst() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DropCfg::clear_has_elementlst() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DropCfg::clear_elementlst() {
  if (elementlst_ != NULL) elementlst_->::com::cfg::vo::DropElement::Clear();
  clear_has_elementlst();
}
inline const ::com::cfg::vo::DropElement& DropCfg::elementlst() const {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  return elementlst_ != NULL ? *elementlst_ : *default_instance().elementlst_;
#else
  return elementlst_ != NULL ? *elementlst_ : *default_instance_->elementlst_;
#endif
}
inline ::com::cfg::vo::DropElement* DropCfg::mutable_elementlst() {
  set_has_elementlst();
  if (elementlst_ == NULL) elementlst_ = new ::com::cfg::vo::DropElement;
  return elementlst_;
}
inline ::com::cfg::vo::DropElement* DropCfg::release_elementlst() {
  clear_has_elementlst();
  ::com::cfg::vo::DropElement* temp = elementlst_;
  elementlst_ = NULL;
  return temp;
}
inline void DropCfg::set_allocated_elementlst(::com::cfg::vo::DropElement* elementlst) {
  delete elementlst_;
  elementlst_ = elementlst;
  if (elementlst) {
    set_has_elementlst();
  } else {
    clear_has_elementlst();
  }
}

// -------------------------------------------------------------------

// DropCfgSet

// repeated .com.cfg.vo.DropCfg dropcfg = 1;
inline int DropCfgSet::dropcfg_size() const {
  return dropcfg_.size();
}
inline void DropCfgSet::clear_dropcfg() {
  dropcfg_.Clear();
}
inline const ::com::cfg::vo::DropCfg& DropCfgSet::dropcfg(int index) const {
  return dropcfg_.Get(index);
}
inline ::com::cfg::vo::DropCfg* DropCfgSet::mutable_dropcfg(int index) {
  return dropcfg_.Mutable(index);
}
inline ::com::cfg::vo::DropCfg* DropCfgSet::add_dropcfg() {
  return dropcfg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::DropCfg >&
DropCfgSet::dropcfg() const {
  return dropcfg_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::DropCfg >*
DropCfgSet::mutable_dropcfg() {
  return &dropcfg_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_DropCfg_2eproto__INCLUDED