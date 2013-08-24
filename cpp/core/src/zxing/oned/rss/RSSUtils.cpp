//
//  RSSUtils.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 2/8/13.
//
//

#include <zxing/oned/rss/RSSUtils.h>

using zxing::oned::rss::RSSUtils;

RSSUtils::RSSUtils() {}

int RSSUtils::getRSSvalue(std::vector<int> &widths, int maxWidth, bool noNarrow) {
    int elements = widths.size();
    int n = 0;
    for (std::vector<int>::iterator it = widths.begin(); it != widths.end(); it++) {
        int width = *it;
        n += width;
    }
    int val = 0;
    int narrowMask = 0;
    for (int bar = 0; bar < elements - 1; bar++) {
        int elmWidth;
        for (elmWidth = 1, narrowMask |= 1 << bar;
             elmWidth < widths[bar];
             elmWidth++, narrowMask &= ~(1 << bar)) {
            int subVal = combins(n - elmWidth - 1, elements - bar - 2);
            if (noNarrow && (narrowMask == 0) &&
                (n - elmWidth - (elements - bar - 1) >= elements - bar - 1)) {
                subVal -= combins(n - elmWidth - (elements - bar),
                                  elements - bar - 2);
            }
            if (elements - bar - 1 > 1) {
                int lessVal = 0;
                for (int mxwElement = n - elmWidth - (elements - bar - 2);
                     mxwElement > maxWidth; mxwElement--) {
                    lessVal += combins(n - elmWidth - mxwElement - 1,
                                       elements - bar - 3);
                }
                subVal -= lessVal * (elements - 1 - bar);
            } else if (n - elmWidth > maxWidth) {
                subVal--;
            }
            val += subVal;
        }
        n -= elmWidth;
    }
    return val;
}

std::vector<int> RSSUtils::getRSSwidths(int val, int n, int elements, int maxWidth, bool noNarrow) {
    std::vector<int> widths (elements, 0);
    int bar;
    int narrowMask = 0;
    for (bar = 0; bar < elements - 1; bar++) {
        narrowMask |= 1 << bar;
        int elmWidth = 1;
        int subVal;
        while (true) {
            subVal = combins(n - elmWidth - 1, elements - bar - 2);
            if (noNarrow && (narrowMask == 0) &&
                (n - elmWidth - (elements - bar - 1) >= elements - bar - 1)) {
                subVal -= combins(n - elmWidth - (elements - bar), elements - bar - 2);
            }
            if (elements - bar - 1 > 1) {
                int lessVal = 0;
                for (int mxwElement = n - elmWidth - (elements - bar - 2);
                     mxwElement > maxWidth;
                     mxwElement--) {
                    lessVal += combins(n - elmWidth - mxwElement - 1, elements - bar - 3);
                }
                subVal -= lessVal * (elements - 1 - bar);
            } else if (n - elmWidth > maxWidth) {
                subVal--;
            }
            val -= subVal;
            if (val < 0) {
                break;
            }
            elmWidth++;
            narrowMask &= ~(1 << bar);
        }
        val += subVal;
        n -= elmWidth;
        widths[bar] = elmWidth;
    }
    widths[bar] = n;
    return widths;
}

int RSSUtils::combins(int n, int r) {
    int maxDenom;
    int minDenom;
    if (n - r > r) {
        minDenom = r;
        maxDenom = n - r;
    } else {
        minDenom = n - r;
        maxDenom = r;
    }
    int val = 1;
    int j = 1;
    for (int i = n; i > maxDenom; i--) {
        val *= i;
        if (j <= minDenom) {
            val /= j;
            j++;
        }
    }
    while (j <= minDenom) {
        val /= j;
        j++;
    }
    return val;
}

std::vector<int> RSSUtils::elements(std::vector<int> &eDist, int N, int K) {
    std::vector<int> widths (eDist.size() + 2, 0);
    int twoK = K << 1;
    widths[0] = 1;
    int i;
    int minEven = 10;
    int barSum = 1;
    for (i = 1; i < twoK - 2; i += 2) {
        widths[i] = eDist[i - 1] - widths[i - 1];
        widths[i + 1] = eDist[i] - widths[i];
        barSum += widths[i] + widths[i + 1];
        if (widths[i] < minEven) {
            minEven = widths[i];
        }
    }
    widths[twoK - 1] = N - barSum;
    if (widths[twoK - 1] < minEven) {
        minEven = widths[twoK - 1];
    }
    if (minEven > 1) {
        for (i = 0; i < twoK; i += 2) {
            widths[i] += minEven - 1;
            widths[i + 1] -= minEven - 1;
        }
    }
    return widths;
}