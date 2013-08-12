//
//  AI01decoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 8/8/13.
//
//

#ifndef __ZXingWidget__AI01decoder__
#define __ZXingWidget__AI01decoder__

#include <zxing/oned/rss/expanded/decoders/AbstractExpandedDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI01decoder : public AbstractExpandedDecoder {
                    private:
                    protected:
                        static const int GTIN_SIZE;
                    public:
                        AI01decoder(Ref<BitArray> information);
                        virtual ~AI01decoder();
                    protected:
                        void encodeCompressedGtin(Ref<String> buf, int currentPos);
                        void encodeCompressedGtinWithoutAI(Ref<String> buf, int currentPos, int initialBufferPosition);
                    private:
                        static void appendCheckDigit(Ref<String> buf, int currentPos);
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI01decoder__) */
