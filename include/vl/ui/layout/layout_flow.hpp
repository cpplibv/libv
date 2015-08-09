// File: LayoutManager.hpp, Created on 2014. január 7. 15:23, Author: Vader

#pragma once

// pro
#include <vl/ui/layout/layout_manager.hpp>

namespace vl {
namespace ui {

class LayoutFlow : public LayoutManager {
public:
	class OrienationImpl;
	typedef const OrienationImpl* Orienation;
	class AlignmentImpl;
	typedef const AlignmentImpl* Alignment;
public:
	//	static Orienation const ORIENTATION_HORIZONTAL;
	//	static Orienation const ORIENTATION_VERTICAL;

	static Alignment const ALIGNMENT_BOTTOM_CENTER;
	static Alignment const ALIGNMENT_BOTTOM_LEFT;
	static Alignment const ALIGNMENT_BOTTOM_RIGHT;
	static Alignment const ALIGNMENT_CENTER_CENTER;
	static Alignment const ALIGNMENT_CENTER_LEFT;
	static Alignment const ALIGNMENT_CENTER_RIGHT;
	static Alignment const ALIGNMENT_TOP_CENTER;
	static Alignment const ALIGNMENT_TOP_LEFT;
	static Alignment const ALIGNMENT_TOP_RIGHT;

	static Orienation const ORIENTATION_UP_LEFT;
	static Orienation const ORIENTATION_UP_RIGHT;
	static Orienation const ORIENTATION_DOWN_LEFT;
	static Orienation const ORIENTATION_DOWN_RIGHT;
	static Orienation const ORIENTATION_LEFT_UP;
	static Orienation const ORIENTATION_LEFT_DOWN;
	static Orienation const ORIENTATION_RIGHT_UP;
	static Orienation const ORIENTATION_RIGHT_DOWN;

private:
	Orienation orientation = ORIENTATION_RIGHT_DOWN;
	Alignment align = ALIGNMENT_TOP_RIGHT;
	Alignment lineAlign = ALIGNMENT_BOTTOM_LEFT;
public:
	LayoutFlow(Orienation orientation, Alignment align, Alignment lineAlign);
	void setOrientation(Orienation orientation) {
		this->orientation = orientation;
	}
	void setAlign(Alignment align) {
		this->align = align;
	}
	void setContentAlign(Alignment lineAlign) {
		this->lineAlign = lineAlign;
	}

public:
//	virtual ivec2 layoutSize(const ProtectedContainer* target);
//	virtual void layoutContainer(ProtectedContainer* target);
	virtual void layout(Container::iterator begin, Container::iterator end, Component* target) override;
};

} //namespace ui
} //namespace vl

//	void addLayoutComponent(std::string name, VComponent comp);
//  void removeLayoutComponent(VComponent comp);
//  ivec2 preferredLayoutSize(VContainer parent);
//  ivec2 minimumLayoutSize(VContainer parent);
//  void layoutContainer(VContainer parent);


//  void addLayoutComponent(Component comp, Object constraints);
//  public Dimension maximumLayoutSize(Container target);
//  public float getLayoutAlignmentX(Container target);
//  public float getLayoutAlignmentY(Container target);
//  public void invalidateLayout(Container target);


///*
// * Defines the interface for classes that know how to lay out
// * <code>Container</code>s.
// * <p>
// * Swing's painting architecture assumes the children of a
// * <code>JComponent</code> do not overlap.  If a
// * <code>JComponent</code>'s <code>LayoutManager</code> allows
// * children to overlap, the <code>JComponent</code> must override
// * <code>isOptimizedDrawingEnabled</code> to return false.
// *
// * @see Container
// * @see javax.swing.JComponent#isOptimizedDrawingEnabled
// *
// * @author      Sami Shaio
// * @author      Arthur van Hoff
// */
//public interface LayoutManager {
//    /**
//     * If the layout manager uses a per-component string,
//     * adds the component <code>comp</code> to the layout,
//     * associating it
//     * with the string specified by <code>name</code>.
//     *
//     * @param name the string to be associated with the component
//     * @param comp the component to be added
//     */
//    void addLayoutComponent(String name, Component comp);
//    /**
//     * Removes the specified component from the layout.
//     * @param comp the component to be removed
//     */
//    void removeLayoutComponent(Component comp);
//    /**
//     * Calculates the preferred size dimensions for the specified
//     * container, given the components it contains.
//     * @param parent the container to be laid out
//     * @see #minimumLayoutSize
//     */
//    Dimension preferredLayoutSize(Container parent);
//
//    /**
//     * Calculates the minimum size dimensions for the specified
//     * container, given the components it contains.
//     * @param parent the component to be laid out
//     * @see #preferredLayoutSize
//     */
//    Dimension minimumLayoutSize(Container parent);
//
//    /**
//     * Lays out the specified container.
//     * @param parent the container to be laid out
//     */
//    void layoutContainer(Container parent);
//}