//
//  BlockParsedResult.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 6/8/13.
//
//

#ifndef __ZXingWidget__BlockParsedResult__
#define __ZXingWidget__BlockParsedResult__

#include <zxing/common/Counted.h>
#include <zxing/oned/rss/expanded/decoders/DecodedInformation.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class BlockParsedResult : public Counted {
                    private:
                        Ref<DecodedInformation> _decodedInformation;
                        bool _finished;
                    public:
                        BlockParsedResult(bool finished);
                        BlockParsedResult(Ref<DecodedInformation> information, bool finished);
                        Ref<DecodedInformation> getDecodedInformation();
                        bool isFinished();
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__BlockParsedResult__) */
