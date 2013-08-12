//
//  AI01320xDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI01320xDecoder__
#define __ZXingWidget__AI01320xDecoder__

#include <zxing/oned/rss/expanded/decoders/AI013x0xDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI01320xDecoder : public AI013x0xDecoder {
                    private:
                    public:
                        AI01320xDecoder(Ref<BitArray> information);
                        virtual ~AI01320xDecoder();
                    protected:
                        void addWeightCode(Ref<String> buf, int weight);
                        int checkWeight(int weight);
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI01320xDecoder__) */
