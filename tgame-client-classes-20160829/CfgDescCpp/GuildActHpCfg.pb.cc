// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: GuildActHpCfg.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "GuildActHpCfg.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace cfg {
namespace vo {

void protobuf_ShutdownFile_GuildActHpCfg_2eproto() {
  delete GuildDrop::default_instance_;
  delete GuildActHpCfg::default_instance_;
  delete GuildActHpCfgSet::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_GuildActHpCfg_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_GuildActHpCfg_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  GuildDrop::default_instance_ = new GuildDrop();
  GuildActHpCfg::default_instance_ = new GuildActHpCfg();
  GuildActHpCfgSet::default_instance_ = new GuildActHpCfgSet();
  GuildDrop::default_instance_->InitAsDefaultInstance();
  GuildActHpCfg::default_instance_->InitAsDefaultInstance();
  GuildActHpCfgSet::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_GuildActHpCfg_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_GuildActHpCfg_2eproto_once_);
void protobuf_AddDesc_GuildActHpCfg_2eproto() {
  ::google::protobuf::::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_GuildActHpCfg_2eproto_once_,
                 &protobuf_AddDesc_GuildActHpCfg_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_GuildActHpCfg_2eproto {
  StaticDescriptorInitializer_GuildActHpCfg_2eproto() {
    protobuf_AddDesc_GuildActHpCfg_2eproto();
  }
} static_descriptor_initializer_GuildActHpCfg_2eproto_;
#endif

// ===================================================================

#ifndef _MSC_VER
const int GuildDrop::kDropItemFieldNumber;
const int GuildDrop::kDropItemProbFieldNumber;
#endif  // !_MSC_VER

GuildDrop::GuildDrop()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void GuildDrop::InitAsDefaultInstance() {
}

GuildDrop::GuildDrop(const GuildDrop& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void GuildDrop::SharedCtor() {
  _cached_size_ = 0;
  dropitem_ = 0u;
  dropitemprob_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

GuildDrop::~GuildDrop() {
  SharedDtor();
}

void GuildDrop::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void GuildDrop::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const GuildDrop& GuildDrop::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_GuildActHpCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_GuildActHpCfg_2eproto();
#endif
  return *default_instance_;
}

GuildDrop* GuildDrop::default_instance_ = NULL;

GuildDrop* GuildDrop::New() const {
  return new GuildDrop;
}

void GuildDrop::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    dropitem_ = 0u;
    dropitemprob_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool GuildDrop::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 dropItem = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &dropitem_)));
          set_has_dropitem();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_dropItemProb;
        break;
      }

      // required uint32 dropItemProb = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_dropItemProb:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &dropitemprob_)));
          set_has_dropitemprob();
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

void GuildDrop::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 dropItem = 1;
  if (has_dropitem()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->dropitem(), output);
  }

  // required uint32 dropItemProb = 2;
  if (has_dropitemprob()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->dropitemprob(), output);
  }

}

int GuildDrop::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 dropItem = 1;
    if (has_dropitem()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->dropitem());
    }

    // required uint32 dropItemProb = 2;
    if (has_dropitemprob()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->dropitemprob());
    }

  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void GuildDrop::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const GuildDrop*>(&from));
}

void GuildDrop::MergeFrom(const GuildDrop& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_dropitem()) {
      set_dropitem(from.dropitem());
    }
    if (from.has_dropitemprob()) {
      set_dropitemprob(from.dropitemprob());
    }
  }
}

void GuildDrop::CopyFrom(const GuildDrop& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GuildDrop::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void GuildDrop::Swap(GuildDrop* other) {
  if (other != this) {
    std::swap(dropitem_, other->dropitem_);
    std::swap(dropitemprob_, other->dropitemprob_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string GuildDrop::GetTypeName() const {
  return "com.cfg.vo.GuildDrop";
}


// ===================================================================

#ifndef _MSC_VER
const int GuildActHpCfg::kLevelFieldNumber;
const int GuildActHpCfg::kHpFieldNumber;
const int GuildActHpCfg::kDropsFieldNumber;
#endif  // !_MSC_VER

GuildActHpCfg::GuildActHpCfg()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void GuildActHpCfg::InitAsDefaultInstance() {
}

GuildActHpCfg::GuildActHpCfg(const GuildActHpCfg& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void GuildActHpCfg::SharedCtor() {
  _cached_size_ = 0;
  level_ = 0u;
  hp_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

GuildActHpCfg::~GuildActHpCfg() {
  SharedDtor();
}

void GuildActHpCfg::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void GuildActHpCfg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const GuildActHpCfg& GuildActHpCfg::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_GuildActHpCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_GuildActHpCfg_2eproto();
#endif
  return *default_instance_;
}

GuildActHpCfg* GuildActHpCfg::default_instance_ = NULL;

GuildActHpCfg* GuildActHpCfg::New() const {
  return new GuildActHpCfg;
}

void GuildActHpCfg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    level_ = 0u;
    hp_ = 0u;
  }
  drops_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool GuildActHpCfg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 level = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &level_)));
          set_has_level();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_hp;
        break;
      }

      // required uint32 hp = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_hp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &hp_)));
          set_has_hp();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_drops;
        break;
      }

      // repeated .com.cfg.vo.GuildDrop drops = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_drops:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_drops()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_drops;
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

void GuildActHpCfg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required uint32 level = 1;
  if (has_level()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->level(), output);
  }

  // required uint32 hp = 2;
  if (has_hp()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->hp(), output);
  }

  // repeated .com.cfg.vo.GuildDrop drops = 3;
  for (int i = 0; i < this->drops_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      3, this->drops(i), output);
  }

}

int GuildActHpCfg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 level = 1;
    if (has_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->level());
    }

    // required uint32 hp = 2;
    if (has_hp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->hp());
    }

  }
  // repeated .com.cfg.vo.GuildDrop drops = 3;
  total_size += 1 * this->drops_size();
  for (int i = 0; i < this->drops_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->drops(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void GuildActHpCfg::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const GuildActHpCfg*>(&from));
}

void GuildActHpCfg::MergeFrom(const GuildActHpCfg& from) {
  GOOGLE_CHECK_NE(&from, this);
  drops_.MergeFrom(from.drops_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_level()) {
      set_level(from.level());
    }
    if (from.has_hp()) {
      set_hp(from.hp());
    }
  }
}

void GuildActHpCfg::CopyFrom(const GuildActHpCfg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GuildActHpCfg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  for (int i = 0; i < drops_size(); i++) {
    if (!this->drops(i).IsInitialized()) return false;
  }
  return true;
}

void GuildActHpCfg::Swap(GuildActHpCfg* other) {
  if (other != this) {
    std::swap(level_, other->level_);
    std::swap(hp_, other->hp_);
    drops_.Swap(&other->drops_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string GuildActHpCfg::GetTypeName() const {
  return "com.cfg.vo.GuildActHpCfg";
}


// ===================================================================

#ifndef _MSC_VER
const int GuildActHpCfgSet::kGuildActHpCfgFieldNumber;
#endif  // !_MSC_VER

GuildActHpCfgSet::GuildActHpCfgSet()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void GuildActHpCfgSet::InitAsDefaultInstance() {
}

GuildActHpCfgSet::GuildActHpCfgSet(const GuildActHpCfgSet& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void GuildActHpCfgSet::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

GuildActHpCfgSet::~GuildActHpCfgSet() {
  SharedDtor();
}

void GuildActHpCfgSet::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void GuildActHpCfgSet::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const GuildActHpCfgSet& GuildActHpCfgSet::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_GuildActHpCfg_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_GuildActHpCfg_2eproto();
#endif
  return *default_instance_;
}

GuildActHpCfgSet* GuildActHpCfgSet::default_instance_ = NULL;

GuildActHpCfgSet* GuildActHpCfgSet::New() const {
  return new GuildActHpCfgSet;
}

void GuildActHpCfgSet::Clear() {
  guildacthpcfg_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool GuildActHpCfgSet::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .com.cfg.vo.GuildActHpCfg guildActHpCfg = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_guildActHpCfg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_guildacthpcfg()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_guildActHpCfg;
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

void GuildActHpCfgSet::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .com.cfg.vo.GuildActHpCfg guildActHpCfg = 1;
  for (int i = 0; i < this->guildacthpcfg_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->guildacthpcfg(i), output);
  }

}

int GuildActHpCfgSet::ByteSize() const {
  int total_size = 0;

  // repeated .com.cfg.vo.GuildActHpCfg guildActHpCfg = 1;
  total_size += 1 * this->guildacthpcfg_size();
  for (int i = 0; i < this->guildacthpcfg_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->guildacthpcfg(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void GuildActHpCfgSet::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const GuildActHpCfgSet*>(&from));
}

void GuildActHpCfgSet::MergeFrom(const GuildActHpCfgSet& from) {
  GOOGLE_CHECK_NE(&from, this);
  guildacthpcfg_.MergeFrom(from.guildacthpcfg_);
}

void GuildActHpCfgSet::CopyFrom(const GuildActHpCfgSet& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GuildActHpCfgSet::IsInitialized() const {

  for (int i = 0; i < guildacthpcfg_size(); i++) {
    if (!this->guildacthpcfg(i).IsInitialized()) return false;
  }
  return true;
}

void GuildActHpCfgSet::Swap(GuildActHpCfgSet* other) {
  if (other != this) {
    guildacthpcfg_.Swap(&other->guildacthpcfg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string GuildActHpCfgSet::GetTypeName() const {
  return "com.cfg.vo.GuildActHpCfgSet";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace vo
}  // namespace cfg
}  // namespace com

// @@protoc_insertion_point(global_scope)
