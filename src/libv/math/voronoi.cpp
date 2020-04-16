// File:   voronoi.cpp Author: Vader Created on 16 April 2020, 12:53

// hpp
#include <libv/math/voronoi.hpp>
// ext
#define JC_VORONOI_IMPLEMENTATION
#include <jc_voronoi/jc_voronoi.h>


namespace libv {
namespace math {

// -------------------------------------------------------------------------------------------------

///**
// * Uses malloc
// * If a clipper is not supplied, a default box clipper will be used
// * If rect is null, an automatic bounding box is calculated, with an extra padding of 10 units
// * All points will be culled against the bounding rect, and all edges will be clipped against it.
// */
//extern void jcv_diagram_generate( int num_points, const jcv_point* points, const jcv_rect* rect, const jcv_clipper* clipper, jcv_diagram* diagram );
//
//// Uses free (or the registered custom free function)
//extern void jcv_diagram_free( jcv_diagram* diagram );
//
//// Returns an array of sites, where each index is the same as the original input point array.
//extern const jcv_site* jcv_diagram_get_sites( const jcv_diagram* diagram );
//
//// Returns a linked list of all the voronoi edges
//// excluding the ones that lie on the borders of the bounding box.
//// For a full list of edges, you need to iterate over the sites, and their graph edges.
//extern const jcv_edge* jcv_diagram_get_edges( const jcv_diagram* diagram );
//
//// Iterates over a list of edges, skipping invalid edges (where p0==p1)
//extern const jcv_edge* jcv_diagram_get_next_edge( const jcv_edge* edge );

void draw_edges(const jcv_diagram* diagram);
void draw_cells(const jcv_diagram* diagram);

void generate_and_draw(int numpoints, const jcv_point* points, int imagewidth, int imageheight) {
	jcv_diagram diagram;
	memset(&diagram, 0, sizeof (jcv_diagram));
	jcv_diagram_generate(count, points, 0, 0, &diagram);

	draw_edges(diagram);
	draw_cells(diagram);

	jcv_diagram_free(&diagram);
}

void draw_edges(const jcv_diagram* diagram) {
	// If all you need are the edges
	const jcv_edge* edge = jcv_diagram_get_edges(diagram);
	while (edge) {
		draw_line(edge->pos[0], edge->pos[1]);
		edge = jcv_diagram_get_next_edge(edge);
	}
}

void draw_cells(const jcv_diagram* diagram) {
	// If you want to draw triangles, or relax the diagram,
	// you can iterate over the sites and get all edges easily
	const jcv_site* sites = jcv_diagram_get_sites(diagram);
	for (int i = 0; i < diagram->numsites; ++i) {
		const jcv_site* site = &sites[i];

		const jcv_graphedge* e = site->edges;
		while (e) {
			draw_triangle(site->p, e->pos[0], e->pos[1]);
			e = e->next;
		}
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace math
} // namespace libv
