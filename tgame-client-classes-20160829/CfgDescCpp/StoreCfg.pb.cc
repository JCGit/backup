// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: StoreCfg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "StoreCfg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace cfg {
namespace vo {

void protobuf_ShutdownFile_StoreCfg_2eproto() {
  delete StoreCfg::default_instance_;
  delete StoreCfgSet::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_StoreCfg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_StoreCfg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  StoreCfg::default_instance_ = new StoreCfg();
  StoreCfgSet::default_instance_ = new StoreCfgSet();
  StoreCfg::default_instance_->InitAsDefaultInstance();
  StoreCfgSet::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_StoreCfg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_StoreCfg_2eproto_once_);
void protobuf_AddDesc_StoreCfg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_StoreCfg_2eproto_once_,
                 &protobuf_AddDesc_StoreCfg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_StoreCfg_2eproto {
  StaticDescriptorInitializer_StoreCfg_2eproto() {
    protobuf_AddDesc_StoreCfg_2eproto();
  }
} static_descriptor_initializer_StoreCfg_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int StoreCfg::kTypeFieldNumber;
const int StoreCfg::kHotFlagFieldNumber;
const int StoreCfg::kCfgIDFieldNumber;
const int StoreCfg::kDetailTypeFieldNumber;
const int StoreCfg::kPriceFieldNumber;
#endif  // !_MSC_VER

StoreCfg::StoreCfg()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void StoreCfg::InitAsDefaultInstance() {
}

StoreCfg::StoreCfg(const StoreCfg& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void StoreCfg::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0u;
  hotflag_ = 0u;
  cfgid_ = 0u;
  detailtype_ = 0u;
  price_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

StoreCfg::~StoreCfg() {
  SharedDtor();
}

void StoreCfg::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void StoreCfg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const StoreCfg& StoreCfg::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_StoreCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_StoreCfg_2eproto();
#endif
  return *default_instance_;
}

StoreCfg* StoreCfg::default_instance_ = NULL;

StoreCfg* StoreCfg::New() const {
  return new StoreCfg;
}

void StoreCfg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0u;
    hotflag_ = 0u;
    cfgid_ = 0u;
    detailtype_ = 0u;
    price_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool StoreCfg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 Type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_HotFlag;
        break;
      }

      // required uint32 HotFlag = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_HotFlag:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &hotflag_)));
          set_has_hotflag();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_CfgID;
        break;
      }

      // required uint32 CfgID = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_CfgID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &cfgid_)));
          set_has_cfgid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_DetailType;
        break;
      }

      // required uint32 DetailType = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_DetailType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &detailtype_)));
          set_has_detailtype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_Price;
        break;
      }

      // required uint32 Price = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_Price:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &price_)));
          set_has_price();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void StoreCfg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 Type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->type(), output);
  }

  // required uint32 HotFlag = 2;
  if (has_hotflag()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->hotflag(), output);
  }

  // required uint32 CfgID = 3;
  if (has_cfgid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->cfgid(), output);
  }

  // required uint32 DetailType = 4;
  if (has_detailtype()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->detailtype(), output);
  }

  // required uint32 Price = 5;
  if (has_price()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->price(), output);
  }

}

int StoreCfg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 Type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->type());
    }

    // required uint32 HotFlag = 2;
    if (has_hotflag()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->hotflag());
    }

    // required uint32 CfgID = 3;
    if (has_cfgid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->cfgid());
    }

    // required uint32 DetailType = 4;
    if (has_detailtype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->detailtype());
    }

    // required uint32 Price = 5;
    if (has_price()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->price());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void StoreCfg::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const StoreCfg*>(&from));
}

void StoreCfg::MergeFrom(const StoreCfg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_hotflag()) {
      set_hotflag(from.hotflag());
    }
    if (from.has_cfgid()) {
      set_cfgid(from.cfgid());
    }
    if (from.has_detailtype()) {
      set_detailtype(from.detailtype());
    }
    if (from.has_price()) {
      set_price(from.price());
    }
  }
}

void StoreCfg::CopyFrom(const StoreCfg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool StoreCfg::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;

  return true;
}

void StoreCfg::Swap(StoreCfg* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(hotflag_, other->hotflag_);
    std::swap(cfgid_, other->cfgid_);
    std::swap(detailtype_, other->detailtype_);
    std::swap(price_, other->price_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string StoreCfg::GetTypeName() const {
  return "com.cfg.vo.StoreCfg";
}


// ===================================================================

#ifndef _MSC_VER
const int StoreCfgSet::kStorecfgFieldNumber;
#endif  // !_MSC_VER

StoreCfgSet::StoreCfgSet()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void StoreCfgSet::InitAsDefaultInstance() {
}

StoreCfgSet::StoreCfgSet(const StoreCfgSet& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void StoreCfgSet::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

StoreCfgSet::~StoreCfgSet() {
  SharedDtor();
}

void StoreCfgSet::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void StoreCfgSet::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const StoreCfgSet& StoreCfgSet::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_StoreCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_StoreCfg_2eproto();
#endif
  return *default_instance_;
}

StoreCfgSet* StoreCfgSet::default_instance_ = NULL;

StoreCfgSet* StoreCfgSet::New() const {
  return new StoreCfgSet;
}

void StoreCfgSet::Clear() {
  storecfg_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool StoreCfgSet::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .com.cfg.vo.StoreCfg storecfg = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_storecfg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_storecfg()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_storecfg;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void StoreCfgSet::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .com.cfg.vo.StoreCfg storecfg = 1;
  for (int i = 0; i < this->storecfg_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->storecfg(i), output);
  }

}

int StoreCfgSet::ByteSize() const {
  int total_size = 0;

  // repeated .com.cfg.vo.StoreCfg storecfg = 1;
  total_size += 1 * this->storecfg_size();
  for (int i = 0; i < this->storecfg_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->storecfg(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void StoreCfgSet::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const StoreCfgSet*>(&from));
}

void StoreCfgSet::MergeFrom(const StoreCfgSet& from) {
  GOOGLE_CHECK_NE(&from, this);
  storecfg_.MergeFrom(from.storecfg_);
}

void StoreCfgSet::CopyFrom(const StoreCfgSet& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool StoreCfgSet::IsInitialized() const {

  for (int i = 0; i < storecfg_size(); i++) {
    if (!this->storecfg(i).IsInitialized()) return false;
  }
  return true;
}

void StoreCfgSet::Swap(StoreCfgSet* other) {
  if (other != this) {
    storecfg_.Swap(&other->storecfg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string StoreCfgSet::GetTypeName() const {
  return "com.cfg.vo.StoreCfgSet";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)