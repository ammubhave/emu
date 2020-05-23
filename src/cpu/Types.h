#pragma once

#include <string_view>

class GeneralPurposeRegister {
 public:
  static const GeneralPurposeRegister AX;
  static const GeneralPurposeRegister CX;
  static const GeneralPurposeRegister DX;
  static const GeneralPurposeRegister BX;
  static const GeneralPurposeRegister SP;
  static const GeneralPurposeRegister BP;
  static const GeneralPurposeRegister SI;
  static const GeneralPurposeRegister DI;
  static const GeneralPurposeRegister AL;
  static const GeneralPurposeRegister CL;
  static const GeneralPurposeRegister DL;
  static const GeneralPurposeRegister BL;
  static const GeneralPurposeRegister AH;
  static const GeneralPurposeRegister CH;
  static const GeneralPurposeRegister DH;
  static const GeneralPurposeRegister BH;

  explicit constexpr GeneralPurposeRegister(uint8_t index) : index_{index} {}

  constexpr bool operator==(const GeneralPurposeRegister& other) const {
    return index_ == other.index_;
  }

  constexpr std::string_view str() const;

  constexpr uint8_t index() const { return index_; }

 private:
  uint8_t index_;
};

constexpr GeneralPurposeRegister GeneralPurposeRegister::AX{0};
constexpr GeneralPurposeRegister GeneralPurposeRegister::CX{1};
constexpr GeneralPurposeRegister GeneralPurposeRegister::DX{2};
constexpr GeneralPurposeRegister GeneralPurposeRegister::BX{3};
constexpr GeneralPurposeRegister GeneralPurposeRegister::SP{4};
constexpr GeneralPurposeRegister GeneralPurposeRegister::BP{5};
constexpr GeneralPurposeRegister GeneralPurposeRegister::SI{6};
constexpr GeneralPurposeRegister GeneralPurposeRegister::DI{7};
constexpr GeneralPurposeRegister GeneralPurposeRegister::AL{0};
constexpr GeneralPurposeRegister GeneralPurposeRegister::CL{1};
constexpr GeneralPurposeRegister GeneralPurposeRegister::DL{2};
constexpr GeneralPurposeRegister GeneralPurposeRegister::BL{3};
constexpr GeneralPurposeRegister GeneralPurposeRegister::AH{4};
constexpr GeneralPurposeRegister GeneralPurposeRegister::CH{5};
constexpr GeneralPurposeRegister GeneralPurposeRegister::DH{6};
constexpr GeneralPurposeRegister GeneralPurposeRegister::BH{7};

constexpr std::string_view GeneralPurposeRegister::str() const {
  switch (index_) {
    case AX.index_:
      return "AX";
    case CX.index_:
      return "CX";
    case DX.index_:
      return "DX";
    case BX.index_:
      return "BX";
    case SP.index_:
      return "SP";
    case BP.index_:
      return "BP";
    case SI.index_:
      return "SI";
    case DI.index_:
      return "DI";
  }
  __builtin_unreachable();
}

class Segment {
 public:
  static const Segment ES;
  static const Segment CS;
  static const Segment SS;
  static const Segment DS;

  explicit constexpr Segment(uint8_t index) : index_{index} {}

  constexpr bool operator==(const Segment& other) const {
    return index_ == other.index_;
  }

  constexpr std::string_view str() const;

  constexpr uint8_t index() const { return index_; }

 private:
  uint8_t index_;
};

constexpr Segment Segment::ES{0};
constexpr Segment Segment::CS{1};
constexpr Segment Segment::SS{2};
constexpr Segment Segment::DS{3};

constexpr std::string_view Segment::str() const {
  switch (index_) {
    case ES.index_:
      return "ES";
    case CS.index_:
      return "CS";
    case SS.index_:
      return "SS";
    case DS.index_:
      return "DS";
  }
  __builtin_unreachable();
}
