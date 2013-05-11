/*----------------------------------------------------------------------------
miniAudicle
Cocoa GUI to chuck audio programming environment

Copyright (c) 2005-2013 Spencer Salazar.  All rights reserved.
http://chuck.cs.princeton.edu/
http://soundlab.cs.princeton.edu/

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: miniAudicleDocument.mm
// desc: Document class, creates a new window for each new document and manages
//       document-level connections to miniAudicle
//
// author: Spencer Salazar (spencer@ccrma.stanford.edu)
// date: Autumn 2005
//-----------------------------------------------------------------------------

#import "miniAudicleDocument.h"
#import "miniAudicleController.h"
#import "NumberedTextView.h"
#import "miniAudiclePreferencesController.h"
#import "miniAudicle.h"
#import "chuck_parse.h"
#import "util_string.h"
#import "NSString+STLString.h"
#import "mADocumentViewController.h"
#import "mAMultiDocWindowController.h"

#import <objc/message.h>


@implementation miniAudicleDocument

@synthesize data;
@synthesize viewController = _viewController;
@synthesize windowController = _windowController;

- (id)init
{
    if( self = [super init] )
    {
        ma = nil;
        
        shows_arguments = YES;
        shows_toolbar = YES;
        shows_line_numbers = YES;
        shows_status_bar = YES;
        
        has_customized_appearance = NO;
    }
    
    return self;
}

- (void)awakeFromNib
{
}

- (void)dealloc
{
    [data release];
    data = nil;
    
    if( ma != nil )
    {
        ma->free_document_id( docid );
        docid = 0;
        ma = nil;
    }
    
    _viewController.document = nil;
    [_viewController release];
    _viewController = nil;
    
    [super dealloc];
}

- (void)makeWindowControllers
{
    miniAudicleController * mac = [NSDocumentController sharedDocumentController];
    mAMultiDocWindowController *_wc = [mac windowControllerForNewDocument];
    [_wc addDocument:self];
    [[_wc window] makeKeyAndOrderFront:self];
//    self.windowController = [mac topWindowController];
}

//- (NSArray *)windowControllers
//{
//    if(_windowController != nil)
//        return @[_windowController];
//    else
//        return @[];
//}
//
- (NSString *)windowNibName
{
    assert(false);
    // Override returning the nib file name of the document
    // If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
    return @"";
}

-(NSViewController *)newPrimaryViewController
{
    mADocumentViewController* ctrl = [[mADocumentViewController alloc] initWithNibName:@"mADocumentView" bundle:nil];
    ctrl.document = self;
    [ctrl setMiniAudicle:ma];
    _viewController = ctrl;
    
    return ctrl;
}

- (NSData *)dataRepresentationOfType:(NSString *)type
{
    return [[_viewController content] dataUsingEncoding:NSASCIIStringEncoding
                                   allowLossyConversion:YES];
}

- (BOOL)loadDataRepresentation:(NSData *)_data ofType:(NSString *)type
{
    data = [[NSString alloc] initWithData:_data encoding:NSASCIIStringEncoding];
    [_viewController setContent:data];
    
    return YES;
}

- (BOOL)isEmpty
{
    return [self.viewController isEmpty] && ![self isDocumentEdited] && [self fileURL] == nil;
}

/* override to set edited/unedited state in window/view controllers */
- (void)updateChangeCount:(NSDocumentChangeType)changeType
{
    [super updateChangeCount:changeType];
    
    if(changeType == NSChangeCleared)
    {
        _viewController.isEdited = NO;
        [_windowController document:self wasEdited:NO];
        [_windowController updateTitles];
    }
    else
    {
        _viewController.isEdited = YES;
        [_windowController document:self wasEdited:YES];
    }
}

///* override to avoid spurious "save changes?" panel when closing the window */
//- (void)shouldCloseWindowController:(NSWindowController *)windowController
//                           delegate:(id)delegate
//                shouldCloseSelector:(SEL)shouldCloseSelector
//                        contextInfo:(void *)contextInfo
//{
//    objc_msgSend(delegate, shouldCloseSelector, self, YES, contextInfo);
//}


- (void)setMiniAudicle:(miniAudicle *)t_ma
{
    ma = t_ma;
    docid = ma->allocate_document_id();
}

@end
