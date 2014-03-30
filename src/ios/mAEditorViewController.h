//
//  mAEditorViewController.h
//  miniAudicle
//
//  Created by Spencer Salazar on 3/26/14.
//
//

#import <UIKit/UIKit.h>

#import "mATitleEditorController.h"
#import "mAKeyboardAccessoryViewController.h"
#import "mAConsoleMonitorController.h"
#import "mASyntaxHighlighter.h"
#import "mADetailViewController.h"

@class mAMasterViewController;
@class mAVMMonitorController;
@class mATextView;

@interface mAEditorViewController : UIViewController
< mAKeyboardAccessoryDelegate,
  UIPopoverControllerDelegate,
  mATitleEditorControllerDelegate,
  NSTextStorageDelegate,
  UITextViewDelegate,
  mADetailClient >
{
    IBOutlet mATextView * _textView;

    IBOutlet mATitleEditorController * _titleEditor;
}

@property (strong, nonatomic) mADetailItem * detailItem;
@property (strong, nonatomic) IBOutlet mAKeyboardAccessoryViewController *keyboardAccessory;
@property (strong, nonatomic) UIBarButtonItem * titleButton;

@property (weak, nonatomic) mAMasterViewController *masterViewController;

- (void)saveScript;

- (IBAction)addShred;
- (IBAction)replaceShred;
- (IBAction)removeShred;

@end