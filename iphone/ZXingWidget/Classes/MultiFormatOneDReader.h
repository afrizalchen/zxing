//
//  MultiFormatOneDReader.h
//  ZXingWidget
//
//  Created by Romain Pechayre on 6/14/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FormatReader.h"
#include <zxing/DecodeHints.h>


@interface MultiFormatOneDReader : FormatReader {
}
- (id) init;
- (id) initWithHints:(zxing::DecodeHints::DecodeHints)hints;
@end
