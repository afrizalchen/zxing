//
//  AI013103decoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#ifndef __ZXingWidget__AI013103decoder__
#define __ZXingWidget__AI013103decoder__

#include <zxing/oned/rss/expanded/decoders/AI013x0xDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AI013103decoder : public AI013x0xDecoder {
                    private:
                    public:
                        AI013103decoder(Ref<BitArray> information);
                        virtual ~AI013103decoder();
                    protected:
                        void addWeightCode(Ref<String> buf, int weight);
                        int checkWeight(int weight);
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AI013103decoder__) */
