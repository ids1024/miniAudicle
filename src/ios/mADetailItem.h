//
//  mADetailItem.h
//  miniAudicle
//
//  Created by Spencer Salazar on 3/27/14.
//
//

#import <Foundation/Foundation.h>

#import "chuck_def.h"

@class mANANewScript;

extern NSString * const mADetailItemTitleChangedNotification;

typedef enum DetailItemType
{
    DETAILITEM_UNDEFINED,
    DETAILITEM_CHUCK_SCRIPT,
    DETAILITEM_AUDIO_FILE,
    DETAILITEM_DIRECTORY,
} DetailItemType;

@interface mADetailItem : NSObject

@property (nonatomic) BOOL isUser;
@property (strong, nonatomic) NSString * title;
@property (strong, nonatomic) NSString * text;
@property (nonatomic) t_CKUINT docid;
@property (nonatomic) BOOL isFolder;
@property (strong, nonatomic) NSMutableArray *folderItems;
@property (strong, nonatomic) NSString *path;
@property (nonatomic) t_CKUINT numShreds;
@property (nonatomic) NSString *uuid;

@property (nonatomic) BOOL remote;
@property (copy, nonatomic) NSString *remoteUUID;
@property (copy, nonatomic) NSString *remoteUsername;
@property (nonatomic) BOOL hasLocalEdits;
@property (nonatomic) DetailItemType type;

+ (mADetailItem *)detailItemFromPath:(NSString *)path isUser:(BOOL)isUser;
+ (mADetailItem *)folderDetailItemWithTitle:(NSString *)title
                                      items:(NSMutableArray *)items
                                     isUser:(BOOL)user;
+ (mADetailItem *)remoteDetailItemWithNewScriptAction:(mANANewScript *)action;

- (NSDictionary *)dictionary;
- (void)save;

@end

