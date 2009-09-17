// Mouse gestures jQuery plugin
// http://shib71.googlepages.com/index.htm
// © 2006 Blair McKenzie | Design by Andreas Viklund
//
// $().gesture(handler,buffer,onchange):
//
// handler
//    Either a function or a hash of gesture-function mappings
// buffer
//    The number of pixels the mouse has to move to trigger a new step in the gesture. Defaults to 25px.
// onchange
//    Set to true to have the handler trigger when the gesture changes instead of when the user releases 
//    their mouse button. While onchange is true, returning false from the handler will stop the gesture. 
//    Defaults to false. 

jQuery.fn.gesture = function(handler, buffer, onchange) {
	
	var bDetecting=false;
	var curGesture;
	var curGesturePos;
	var fullGesture;
	var onchange = onchange || (buffer && buffer.constructor == Boolean ? buffer : false)
	var buffer = buffer && buffer.constructor == Number ? buffer*buffer : 625;
	var self = this;
	var startelement=undefined;
	
	var blockSelect = function() { return false; };
	var processGesture = function(gesture) {
		if (gesture !== '' && handler.constructor == Function) // Then all gestures are handled by this function
			return handler.apply(startelement,[ gesture ]);
		else if (gesture !== '' && handler.constructor == Object) // Then each gesture has its own function
			if (handler[gesture]) return handler[gesture].apply(startelement,[gesture]);
	};
	var closeGesture = function() {
		bDetecting = false;
		curGesture = '-';
		curGesturePos = {  };
		fullGesture = '';
		startelement=undefined;
		jquery(document).unbind("selectstart",blockSelect);
		jquery(document).unmousedown(blockSelect);
	}
	var mdGesture=function(event) {
		var x = event.pageX || (event.clientX + (document.documentElement.scrollLeft || document.body.scrollLeft)) || 0;
		var y = event.pageY || (event.clientY + (document.documentElement.scrollTop || document.body.scrollTop)) || 0;
		
		bDetecting = true;
		curGesture = '-';
		curGesturePos = { x:x, y:y };
		fullGesture = '';
		startelement=this;
		jquery(document).bind("selectstart",blockSelect);
		jquery(document).mousedown(blockSelect);
	};
	var mmGesture=function(event) {
		var thisGesture;
		
		if (bDetecting ) {
 		// Get distance from origonal position
 		var pointer = { x:event.pageX || (event.clientX + (document.documentElement.scrollLeft || document.body.scrollLeft)) || 0,
 		                y:event.pageY || (event.clientY + (document.documentElement.scrollTop || document.body.scrollTop)) || 0 };
 		var r = (curGesturePos.x-pointer.x)*(curGesturePos.x-pointer.x) + (pointer.y-curGesturePos.y)*(pointer.y-curGesturePos.y)
 		
			if (r>buffer) {
 			// Get gesture
 			if (Math.abs(curGesturePos.x-pointer.x) > Math.abs(curGesturePos.y-pointer.y))
 				thisGesture = curGesturePos.x-pointer.x > 0 ? 'L' : 'R';
 			else
 				thisGesture = curGesturePos.y-pointer.y > 0 ? 'U' : 'D';
 				
 			// If this gesture is different from the current gesture, update the full gesture
 			if (curGesture !== thisGesture) {
 				curGesture = thisGesture;
 				fullGesture += thisGesture;
 				
 				// if the handler is supposed to go off on gesture change AND mouse up...
 				// process the gesture. use the return value of the function to determine
 				// whether the gesture is killed or whether it allowed to continue
 				// ie if the user continues moving the mouse, is that another gesture?
 				if (onchange && processGesture(fullGesture)===false) closeGesture();
 			}
 			
 			// Set current position to this position
 			curGesturePos = { "x":pointer.x, "y":pointer.y };
			}
			
			return false;
		}
	};
	var muGesture=function() {
		if (bDetecting ) {
			if (onchange != true) processGesture(fullGesture);
			closeGesture();
		}
	};
	
	// Remove the functions from 'this' if they are already attacted
	// Needed to handle multiple calls on a single object
	// and when the user simply wants to remove gestures (handler == undefined)
	this.unmousedown(mdGesture);
	$(document).unmousemove(mmGesture).unmouseup(muGesture);
	
	// Attach gesture handlers
	if (handler) {
		this.mousedown(mdGesture);
		$(document).mousemove(mmGesture).mouseup(muGesture);
	}
	
	return this;
};