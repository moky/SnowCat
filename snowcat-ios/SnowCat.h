//
//  SnowCat.h
//  SnowCat
//
//  Created by Moky on 14-3-28.
//  Copyright (c) 2014 Moky. All rights reserved.
//

/**
 *  Dependences:
 *
 *      <Foundation.framework>
 *      <CoreGraphics.framework>
 *      <UIKit.framework>
 *      <QuartzCore.framework>
 *
 */

// common
#import "scMacros.h"
#import "SCLog.h"
//#import "SCMath.h"
#import "SCClient.h"
#import "SCMemoryCache.h"
#import "SCNodeFileParser.h"
#import "SCEventHandler.h"
#import "SCEventDispatcher.h"
#import "SCDataLoader.h"

// data
#import "SCMemoryObjectFile.h"

// fsm
#import "SCFiniteStateMachine.h"

// foundation
#import "SCObject.h"
#import "SCString.h"
#import "SCString+Extension.h"
#import "SCDate+Extension.h"
#import "SCDictionary.h"
#import "SCURL.h"
#import "SCURLRequest.h"
#import "SCFileManager.h"
#import "SCBundle.h"
#import "SCIndexPath.h"
#import "SCAttributedString.h"

// uikit
#import "SCUIKit.h"
#import "SCGeometry.h"
#import "SCNib.h"
#import "SCColor.h"
#import "SCFont.h"
#import "SCImage.h"
#import "SCResponder.h"
#import "SCWindow.h"
#import "SCApplication.h"
#import "SCDevice.h"
// uikit/items
#import "SCBarItem.h"
#import "SCBarButtonItem.h"
#import "SCTabBar.h"
#import "SCTabBarItem.h"
#import "SCNavigationBar.h"
#import "SCNavigationItem.h"
// uikit/views
#import "SCView.h"
#import "SCView+Geometry.h"
#import "SCView+Gesture.h"
#import "SCControl.h"
#import "SCButton.h"
#import "SCLabel.h"
#import "SCTextField.h"
#import "SCSearchBar.h"
#import "SCToolbar.h"
#import "SCProgressView.h"
#import "SCSegmentedControl.h"
#import "SCSlider.h"
#import "SCStepper.h"
#import "SCSwitch.h"
#import "SCImageView.h"
#import "SCScrollView.h"
#import "SCTextView.h"
#import "SCWebView.h"
#import "SCVisualEffectView.h"
// uikit/views/table
#import "SCTableView.h"
#import "SCTableViewCell.h"
#import "SCTableViewHeaderFooterView.h"
// uikit/views/collection
#import "SCCollectionView.h"
#import "SCCollectionReusableView.h"
#import "SCCollectionViewCell.h"
// uikit/views/collection/layout
#import "SCCollectionViewLayout.h"
#import "SCCollectionViewFlowLayout.h"
#import "SCCollectionViewLayoutAttributes.h"
// uikit/views/widgets
#import "SCPageControl.h"
#import "SCRefreshControl.h"
#import "SCActivityIndicatorView.h"
#import "SCAlertView.h"
#import "SCActionSheet.h"
#import "SCDatePicker.h"
#import "SCPickerView.h"
// uikit/controllers
#import "SCViewController.h"
#import "SCNavigationController.h"
#import "SCTabBarController.h"
#import "SCTableViewController.h"
#import "SCCollectionViewController.h"
#import "SCPageViewController.h"
#import "SCSplitViewController.h"
#import "SCAlertController.h"
// uikit/delegates
#import "SCActionSheetDelegate.h"
#import "SCAlertViewDelegate.h"
#import "SCCollectionViewDelegate.h"
#import "SCCollectionViewDelegateFlowLayout.h"
#import "SCCollectionViewDataSource.h"
#import "SCNavigationControllerDelegate.h"
#import "SCPageViewControllerDataSource.h"
#import "SCPageViewControllerDelegate.h"
#import "SCPickerViewDataHandler.h"
#import "SCPickerViewDataSource.h"
#import "SCPickerViewDelegate.h"
#import "SCScrollViewDelegate.h"
#import "SCSearchBarDelegate.h"
#import "SCSplitViewControllerDelegate.h"
#import "SCTabBarDelegate.h"
#import "SCTabBarControllerDelegate.h"
#import "SCTableViewDataHandler.h"
#import "SCTableViewDataSource.h"
#import "SCTableViewDelegate.h"
#import "SCTextFieldDelegate.h"
#import "SCTextViewDelegate.h"
#import "SCWebViewDelegate.h"
// uikit/utils
#import "SCInterface.h"
#import "SCStringDrawing.h"
#import "SCTextInputTraits.h"
#import "SCTextInput.h"
#import "SCText.h"
#import "SCParagraphStyle.h"
#import "SCDataDetectors.h"
// uikit/actions
#import "SCAction.h"
#import "SCActionCallFunc.h"
#import "SCActionAlert.h"
#import "SCActionActionSheet.h"
#import "SCActionNotification.h"
#import "SCActionView.h"
#import "SCActionViewGeometry.h"
#import "SCActionViewController.h"
// uikit/extensions
#import "SCAnimationView.h"
#import "SCSwitchButton.h"
#import "SCGroundView.h"
#import "SCDragView.h"
#import "SCBlurView.h"
#import "SCRemoteController.h"
// uikit/extensions/particle
#import "SCParticleView.h"
#import "SCTouchParticleView.h"
// uikit/extensions/page
#import "SCPageScrollViewDataSource.h"
#import "SCPageScrollView.h"
#import "SCPrismScrollView.h"
#import "SCDockScrollView.h"
#import "SCCoverFlowView.h"
// uikit/extensions/table
#import "SCComplexTableView.h"
#import "SCComplexTableViewDataSource.h"
#import "SCComplexTableViewDelegate.h"
#import "SCGridTableView.h"
#import "SCGridTableViewDataSource.h"
#import "SCGridTableViewDelegate.h"
#import "SCSwipeTableViewCell.h"
// uikit/extensions/refresh
#import "SCScrollRefreshControlState.h"
#import "SCScrollRefreshControl.h"
#import "SCScrollRefreshView.h"
// uikit/extensions/web
#import "SCWebView+JSBridge.h"
#import "SCWebViewDelegate+JSBridge.h"

// quartz core
#import "SCAffineTransform.h"
#import "SCEmitterCell.h"
#import "SCEmitterLayer.h"
#import "SCLayer.h"
#import "SCTransition.h"
#import "SCView+Reflection.h"
#import "SCView+Transform.h"

NSString * snowcatVersion(void);
