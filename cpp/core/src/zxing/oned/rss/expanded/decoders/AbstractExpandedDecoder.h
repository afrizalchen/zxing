//
//  AbstractExpandedDecoder.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 4/8/13.
//
//

#ifndef __ZXingWidget__AbstractExpandedDecoder__
#define __ZXingWidget__AbstractExpandedDecoder__

#import <zxing/common/Counted.h>
#import <zxing/common/BitArray.h>
#import <zxing/oned/rss/expanded/decoders/GeneralAppIdDecoder.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class AbstractExpandedDecoder : public Counted{
                    private:
                        Ref<BitArray> _information;
                        Ref<GeneralAppIdDecoder> _generalDecoder;
                    public:
                        AbstractExpandedDecoder(Ref<BitArray> information);
                        virtual ~AbstractExpandedDecoder();
                        virtual Ref<String> parseInformation() = 0;
                        static Ref<AbstractExpandedDecoder> createDecoder(Ref<BitArray> information);
                    protected:
                        Ref<BitArray> getInformation();
                        Ref<GeneralAppIdDecoder> getGeneralDecoder();
                        
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__AbstractExpandedDecoder__) */
