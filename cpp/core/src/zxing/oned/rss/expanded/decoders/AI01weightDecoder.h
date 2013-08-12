//
//  AI01weightDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI01weightDecoder__
#define __ZXingWidget__AI01weightDecoder__

#include <zxing/oned/rss/expanded/decoders/AI01decoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI01weightDecoder : public AI01decoder {
                    private:
                    public:
                        AI01weightDecoder(Ref<BitArray> information);
                        virtual ~AI01weightDecoder();
                    protected:
                        void encodeCompressedWeight(Ref<String> buf, int currentPos, int weightSize);
                        virtual void addWeightCode(Ref<String> buf, int weight) = 0;
                        virtual int checkWeight(int weight) = 0;
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI01weightDecoder__) */
