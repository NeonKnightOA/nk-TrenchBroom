//
//  Vector3i.m
//  TrenchBroom
//
//  Created by Kristian Duske on 30.01.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Vector3i.h"
#import "Math3D.h"

@implementation Vector3i

- (id)init {
	if (self = [super init]) {
        coords[0] = 0;
        coords[1] = 0;
        coords[2] = 0;
	}
	
	return self;
}

- (id)initWithVector:(Vector3i *)vector {
	if (vector == nil) {
		[self release];
		return nil;
	}
	
	if (self = [super init])
		[self set:vector];
	
	return self;
}

- (id)initWithXCoord:(int)xCoord yCoord:(int)yCoord zCoord:(int)zCoord {
	if (self = [super init]) {
		[self setX:xCoord];
		[self setY:yCoord];
		[self setZ:zCoord];
	}
	
	return self;
}

- (int)x {
	return coords[0];
}

- (int)y {
	return coords[1];
}

- (int)z {
	return coords[2];
}

- (void)setX:(int)xCoord {
	coords[0] = xCoord;
}

- (void)setY:(int)yCoord {
	coords[1] = yCoord;
}

- (void)setZ:(int)zCoord {
	coords[2] = zCoord;
}

- (void)set:(Vector3i *)vector {
	[self setX:[vector x]];
	[self setY:[vector y]];
	[self setZ:[vector z]];
}

- (BOOL)null {
    return coords[0] == 0 && coords[1] == 0 && coords[2] == 0;
}

- (void)add:(Vector3i *)addend {
    coords[0] += [addend x];
    coords[1] += [addend y];
    coords[2] += [addend z];
}

- (void)addX:(int)xAddend Y:(int)yAddend Z:(int)zAddend {
    coords[0] += xAddend;
    coords[1] += yAddend;
    coords[2] += zAddend;
}

- (void)cross:(Vector3i *)m {
    int x = [self y] * [m z] - [self z] * [m y];
    int y = [self z] * [m x] - [self x] * [m z];
    int z = [self x] * [m y] - [self y] * [m x];
    
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
}

- (BOOL)isEqual:(id)object {
    if (object == self)
        return YES;
    
    if (![object isKindOfClass:[self class]])
        return NO;
    
    Vector3i* vector = (Vector3i*)object;
    return fabsf([self x] - [vector x]) <= AlmostZero && fabsf([self y] - [vector y]) <= AlmostZero && fabsf([self z] - [vector z]) <= AlmostZero;
}

- (NSString *)description {
    return [NSString stringWithFormat:@"X: %i, Y: %i, Z: %i", [self x], [self y], [self z]];
}

@end
