// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CostCfg.proto

#ifndef PROTOBUF_CostCfg_2eproto__INCLUDED
#define PROTOBUF_CostCfg_2eproto__INCLUDED

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
void  protobuf_AddDesc_CostCfg_2eproto();
void protobuf_AssignDesc_CostCfg_2eproto();
void protobuf_ShutdownFile_CostCfg_2eproto();

class CostCfg;
class CostCfgSet;

// ===================================================================

class CostCfg : public ::google::protobuf::MessageLite {
 public:
  CostCfg();
  virtual ~CostCfg();

  CostCfg(const CostCfg& from);

  inline CostCfg& operator=(const CostCfg& from) {
    CopyFrom(from);
    return *this;
  }

  static const CostCfg& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const CostCfg* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(CostCfg* other);

  // implements Message ----------------------------------------------

  CostCfg* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const CostCfg& from);
  void MergeFrom(const CostCfg& from);
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

  // required uint32 Count = 1;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 1;
  inline ::google::protobuf::uint32 count() const;
  inline void set_count(::google::protobuf::uint32 value);

  // required uint32 BuyManualCost = 2;
  inline bool has_buymanualcost() const;
  inline void clear_buymanualcost();
  static const int kBuyManualCostFieldNumber = 2;
  inline ::google::protobuf::uint32 buymanualcost() const;
  inline void set_buymanualcost(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:com.cfg.vo.CostCfg)
 private:
  inline void set_has_count();
  inline void clear_has_count();
  inline void set_has_buymanualcost();
  inline void clear_has_buymanualcost();

  ::google::protobuf::uint32 count_;
  ::google::protobuf::uint32 buymanualcost_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_CostCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_CostCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_CostCfg_2eproto();
  friend void protobuf_ShutdownFile_CostCfg_2eproto();

  void InitAsDefaultInstance();
  static CostCfg* default_instance_;
};
// -------------------------------------------------------------------

class CostCfgSet : public ::google::protobuf::MessageLite {
 public:
  CostCfgSet();
  virtual ~CostCfgSet();

  CostCfgSet(const CostCfgSet& from);

  inline CostCfgSet& operator=(const CostCfgSet& from) {
    CopyFrom(from);
    return *this;
  }

  static const CostCfgSet& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const CostCfgSet* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(CostCfgSet* other);

  // implements Message ----------------------------------------------

  CostCfgSet* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const CostCfgSet& from);
  void MergeFrom(const CostCfgSet& from);
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

  // repeated .com.cfg.vo.CostCfg buycostcfg = 1;
  inline int buycostcfg_size() const;
  inline void clear_buycostcfg();
  static const int kBuycostcfgFieldNumber = 1;
  inline const ::com::cfg::vo::CostCfg& buycostcfg(int index) const;
  inline ::com::cfg::vo::CostCfg* mutable_buycostcfg(int index);
  inline ::com::cfg::vo::CostCfg* add_buycostcfg();
  inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::CostCfg >&
      buycostcfg() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::CostCfg >*
      mutable_buycostcfg();

  // @@protoc_insertion_point(class_scope:com.cfg.vo.CostCfgSet)
 private:

  ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::CostCfg > buycostcfg_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_CostCfg_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_CostCfg_2eproto();
  #endif
  friend void protobuf_AssignDesc_CostCfg_2eproto();
  friend void protobuf_ShutdownFile_CostCfg_2eproto();

  void InitAsDefaultInstance();
  static CostCfgSet* default_instance_;
};
// ===================================================================


// ===================================================================

// CostCfg

// required uint32 Count = 1;
inline bool CostCfg::has_count() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CostCfg::set_has_count() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CostCfg::clear_has_count() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CostCfg::clear_count() {
  count_ = 0u;
  clear_has_count();
}
inline ::google::protobuf::uint32 CostCfg::count() const {
  return count_;
}
inline void CostCfg::set_count(::google::protobuf::uint32 value) {
  set_has_count();
  count_ = value;
}

// required uint32 BuyManualCost = 2;
inline bool CostCfg::has_buymanualcost() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CostCfg::set_has_buymanualcost() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CostCfg::clear_has_buymanualcost() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CostCfg::clear_buymanualcost() {
  buymanualcost_ = 0u;
  clear_has_buymanualcost();
}
inline ::google::protobuf::uint32 CostCfg::buymanualcost() const {
  return buymanualcost_;
}
inline void CostCfg::set_buymanualcost(::google::protobuf::uint32 value) {
  set_has_buymanualcost();
  buymanualcost_ = value;
}

// -------------------------------------------------------------------

// CostCfgSet

// repeated .com.cfg.vo.CostCfg buycostcfg = 1;
inline int CostCfgSet::buycostcfg_size() const {
  return buycostcfg_.size();
}
inline void CostCfgSet::clear_buycostcfg() {
  buycostcfg_.Clear();
}
inline const ::com::cfg::vo::CostCfg& CostCfgSet::buycostcfg(int index) const {
  return buycostcfg_.Get(index);
}
inline ::com::cfg::vo::CostCfg* CostCfgSet::mutable_buycostcfg(int index) {
  return buycostcfg_.Mutable(index);
}
inline ::com::cfg::vo::CostCfg* CostCfgSet::add_buycostcfg() {
  return buycostcfg_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::CostCfg >&
CostCfgSet::buycostcfg() const {
  return buycostcfg_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::cfg::vo::CostCfg >*
CostCfgSet::mutable_buycostcfg() {
  return &buycostcfg_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_CostCfg_2eproto__INCLUDED