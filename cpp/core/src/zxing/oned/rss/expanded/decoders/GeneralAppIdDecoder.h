//
//  GeneralAppIdDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 6/8/13.
//
//

#ifndef __ZXingWidget__GeneralAppIdDecoder__
#define __ZXingWidget__GeneralAppIdDecoder__

#include <zxing/common/Counted.h>
#include <zxing/common/BitArray.h>
#include <zxing/common/Str.h>
#include <zxing/oned/rss/expanded/decoders/BlockParsedResult.h>
#include <zxing/oned/rss/expanded/decoders/CurrentParsingState.h>
#include <zxing/oned/rss/expanded/decoders/DecodedChar.h>
#include <zxing/oned/rss/expanded/decoders/DecodedInformation.h>
#include <zxing/oned/rss/expanded/decoders/DecodedNumeric.h>


namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class GeneralAppIdDecoder : public Counted {
                    private:
                        Ref<BitArray> _information;
                        Ref<CurrentParsingState> _current;
                        Ref<String> _buffer;
                    public:
                        GeneralAppIdDecoder(Ref<BitArray> information);
                        Ref<String> decodeAllCodes(Ref<String> buff, int initialPosition);
                        int extractNumericValueFromBitArray(int pos, int bits);
                        static int extractNumericValueFromBitArray(Ref<BitArray> information, int pos, int bits);
                        Ref<DecodedInformation> decodeGeneralPurposeField(int pos, Ref<String> remaining);
                    private:
                        bool isStillNumeric(int pos);
                        Ref<DecodedNumeric> decodeNumeric(int pos);
                        Ref<DecodedInformation> parseBlocks();
                        Ref<BlockParsedResult> parseNumericBlock();
                        Ref<BlockParsedResult> parseIsoIec646Block();
                        Ref<BlockParsedResult> parseAlphaBlock();
                        bool isStillIsoIec646(int pos);
                        Ref<DecodedChar> decodeIsoIec646(int pos);
                        bool isStillAlpha(int pos);
                        Ref<DecodedChar> decodeAlphanumeric(int pos);
                        bool isAlphaTo646ToAlphaLatch(int pos);
                        bool isAlphaOr646ToNumericLatch(int pos);
                        bool isNumericToAlphaNumericLatch(int pos);
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__GeneralAppIdDecoder__) */
