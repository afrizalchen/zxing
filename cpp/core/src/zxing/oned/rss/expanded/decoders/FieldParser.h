//
//  FieldParser.h
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#ifndef __ZXingWidget__FieldParser__
#define __ZXingWidget__FieldParser__

#include <zxing/common/Str.h>

namespace zxing {
    namespace oned {
        namespace rss {
            namespace expanded {
                namespace decoders {
                    class FieldParser {
                    private:
                    private:
                        FieldParser();
                        static Ref<String> processFixedAI(int aiSize, int fieldSize, Ref<String> rawInformation);
                        static Ref<String> processVariableAI(int aiSize, int variableFieldSize, Ref<String> rawInformation);
                    public:
                        static Ref<String> parseFieldsInGeneralPurpose(Ref<String> rawInformation);
                        
                    };
                }
            }
        }
    }
}

#endif /* defined(__ZXingWidget__FieldParser__) */
