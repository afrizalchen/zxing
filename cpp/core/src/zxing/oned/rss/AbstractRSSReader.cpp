//
//  AbstractRSSReader.cpp
//  zxing-cpp
//
//  Created by Afrizal Chen on 3/8/13.
//
//

#include <numeric>
#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/AbstractRSSReader.h>
#include <zxing/oned/rss/FinderPattern.h>

using zxing::oned::rss::AbstractRSSReader;
using zxing::oned::rss::FinderPattern;

namespace {
    
}

const int AbstractRSSReader::MAX_AVG_VARIANCE =
  (int) (PATTERN_MATCH_RESULT_SCALE_FACTOR * 2.0f);
const int AbstractRSSReader::MAX_INDIVIDUAL_VARIANCE =
  (int) (PATTERN_MATCH_RESULT_SCALE_FACTOR * 0.45f);
const float AbstractRSSReader::MIN_FINDER_PATTERN_RATIO = 9.5f / 12.0f;
const float AbstractRSSReader::MAX_FINDER_PATTERN_RATIO = 12.5f / 14.0f;

AbstractRSSReader::AbstractRSSReader() {
    decodeFinderCounters = std::vector<int> (4, 0);
    dataCharacterCounters = std::vector<int> (8, 0);
    oddRoundingErrors = std::vector<float> (4, 0);
    evenRoundingErrors = std::vector<float> (4, 0);
    oddCounts = std::vector<int> (dataCharacterCounters.size() / 2, 0);
    evenCounts = std::vector<int> (dataCharacterCounters.size() / 2, 0);
}

AbstractRSSReader::~AbstractRSSReader() {
}

std::vector<int> &AbstractRSSReader::getDecodeFinderCounters() {
    return decodeFinderCounters;
}

std::vector<int> &AbstractRSSReader::getDataCharacterCounters() {
    return dataCharacterCounters;
}


std::vector<float> &AbstractRSSReader::getOddRoundingErrors() {
    return oddRoundingErrors;
}


std::vector<float> &AbstractRSSReader::getEvenRoundingErrors() {
    return evenRoundingErrors;
}


std::vector<int> &AbstractRSSReader::getOddCounts() {
    return oddCounts;
}


std::vector<int> &AbstractRSSReader::getEvenCounts() {
    return evenCounts;
}



int AbstractRSSReader::parseFinderValue(std::vector<int> &counters, std::vector<int const*> const& finderPatterns) {
    int finderPatternCount = finderPatterns.size();
    for (int value = 0; value < finderPatternCount; value++) {
        if (patternMatchVariance(counters, finderPatterns[value], MAX_INDIVIDUAL_VARIANCE) <
            MAX_AVG_VARIANCE) {
            return value;
        }
    }
    throw NotFoundException();
}


int AbstractRSSReader::count(std::vector<int> &array) {
    int count = 0;
    for (int a : array) {
        count += a;
    }
    return count;
}


void AbstractRSSReader::increment(std::vector<int> &array, std::vector<float> &errors) {
    int index = 0;
    float biggestError = errors[0];
    int arrayCount = array.size();
    for (int i = 1; i < arrayCount; i++) {
        if (errors[i] > biggestError) {
            biggestError = errors[i];
            index = i;
        }
    }
    array[index]++;
}


void AbstractRSSReader::decrement(std::vector<int> &array, std::vector<float> &errors) {
    int index = 0;
    float biggestError = errors[0];
    int arrayCount = array.size();
    for (int i = 1; i < arrayCount; i++) {
        if (errors[i] < biggestError) {
            biggestError = errors[i];
            index = i;
        }
    }
    array[index]--;
}


bool AbstractRSSReader::isFinderPattern(std::vector<int> &counters) {
    int firstTwoSum = counters[0] + counters[1];
    int sum = firstTwoSum + counters[2] + counters[3];
    float ratio = (float) firstTwoSum / (float) sum;
    if (ratio >= MIN_FINDER_PATTERN_RATIO && ratio <= MAX_FINDER_PATTERN_RATIO) {
        // passes ratio test in spec, but see if the counts are unreasonable
        int minCounter = std::numeric_limits<int>::max();
        int maxCounter = std::numeric_limits<int>::min();
        for (int counter : counters) {
            if (counter > maxCounter) {
                maxCounter = counter;
            }
            if (counter < minCounter) {
                minCounter = counter;
            }
        }
        return maxCounter < 10 * minCounter;
    }
    return false;
}
