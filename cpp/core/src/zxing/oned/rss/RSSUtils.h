//
//  RSSUtils.h
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#ifndef __zxing_cpp__RSSUtils__
#define __zxing_cpp__RSSUtils__

#include <vector>

namespace zxing {
    namespace oned {
        namespace rss {
            class RSSUtils {
            private:
                RSSUtils();
            public:
                static int getRSSvalue(std::vector<int> &widths, int maxWidth, bool noNarrow);
            private:
                static std::vector<int> getRSSwidths(int val, int n, int elements, int maxWidth, bool noNarrow);
                static int combins(int n, int r);
                static std::vector<int> elements(std::vector<int> &eDist, int N, int K);
            };
            
        }
    }
}

#endif /* defined(__zxing_cpp__RSSUtils__) */
