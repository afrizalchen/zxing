//
//  AnyAIDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 8/8/13.
//
//

#ifndef __ZXingWidget__AnyAIDecoder__
#define __ZXingWidget__AnyAIDecoder__

#include <zxing/oned/rss/expanded/decoders/AbstractExpandedDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AnyAIDecoder : public AbstractExpandedDecoder {
                    private:
                    public:
                        AnyAIDecoder(Ref<BitArray> information);
                        virtual ~AnyAIDecoder();
                        virtual Ref<String> parseInformation();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AnyAIDecoder__) */
