//
//  AI013x0x1xDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI013x0x1xDecoder__
#define __ZXingWidget__AI013x0x1xDecoder__

#include <zxing/oned/rss/expanded/decoders/AI01weightDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI013x0x1xDecoder : public AI01weightDecoder {
                    private:
                        Ref<String> _dateCode;
                        Ref<String> _firstAIdigits;
                    public:
                        AI013x0x1xDecoder(Ref<BitArray> information, Ref<String> firstAIdigits, Ref<String> dateCode);
                        virtual ~AI013x0x1xDecoder();
                        Ref<String> parseInformation();
                    private:
                        void encodeCompressedDate(Ref<String> buf, int currentPos);
                        void addWeightCode(Ref<String> buf, int weight);
                        int checkWeight(int weight);
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI013x0x1xDecoder__) */
