// Copyright (C) 2000  Sean Cavanaugh
// This file is licensed under the terms of the Lesser GNU Public License
// (see LPGL.txt, or http://www.gnu.org/copyleft/lesser.txt)

#include "vis.h"


void Zones::set( uint32_t zone, const BSPBoundingBox& bounds )
{
        if ( zone < m_ZoneCount )
        {
                m_ZoneBounds[zone] = bounds;
        }
}

uint32_t Zones::getZoneFromBounds( const BSPBoundingBox& bounds )
{
        uint32_t x;
        for ( x = 0; x<m_ZoneCount; x++ )
        {
                if ( m_ZoneBounds[x].testSuperset( bounds ) )
                {
                        return x;
                }
        }
        return 0;
}

uint32_t Zones::getZoneFromWinding( const Winding& winding )
{
        uint32_t          x;
        BSPBoundingBox     bounds;

        for ( x = 0; x<winding.m_NumPoints; x++ )
        {
                bounds.add( winding.m_Points[x] );
        }

        return getZoneFromBounds( bounds );
}

// BORROWED FROM HLRAD
// TODO: Consolite into common sometime
static Winding*      WindingFromFace( const dface_t* f )
{
        int             i;
        int             se;
        dvertex_t*      dv;
        int             v;
        Winding*        w = new Winding( f->numedges );

        for ( i = 0; i < f->numedges; i++ )
        {
                se = g_bspdata->dsurfedges[f->firstedge + i];
                if ( se < 0 )
                {
                        v = g_bspdata->dedges[-se].v[1];
                }
                else
                {
                        v = g_bspdata->dedges[se].v[0];
                }

                dv = &g_bspdata->dvertexes[v];
                VectorCopy( dv->point, w->m_Points[i] );
        }

        return w;
}

Zones* MakeZones( void )
{
        uint32_t x;
        uint32_t func_vis_count = 0;

        ParseEntities( g_bspdata );

        // Note: we arent looping through entities because we only care if it has a winding/bounding box

        // First count the number of func_vis's
        for ( x = 0; x<g_bspdata->nummodels; x++ )
        {
                entity_t*       ent = EntityForModel( g_bspdata, x );

                if ( !strcasecmp( ValueForKey( ent, "classname" ), "func_vis" ) )
                {
                        uint32_t value = atoi( ValueForKey( ent, "node" ) );
                        if ( value )
                        {
                                func_vis_count++;
                        }
                        else
                        {
                                Error( "func_vis with no \"node\" id\n" );
                        }
                }
        }

        if ( !func_vis_count )
        {
                return NULL;
        }

        Zones* zones = new Zones( func_vis_count );

        for ( x = 0; x<g_bspdata->nummodels; x++ )
        {
                dmodel_t*       mod = g_bspdata->dmodels + x;
                entity_t*       ent = EntityForModel( g_bspdata, x );

                if ( !strcasecmp( ValueForKey( ent, "classname" ), "func_vis" ) )
                {
                        uint32_t func_vis_id = atoi( ValueForKey( ent, "node" ) );

                        {
                                epair_t* keyvalue;

                                for ( keyvalue = ent->epairs; keyvalue; keyvalue = keyvalue->next )
                                {
                                        uint32_t other_id = atoi( keyvalue->key );
                                        if ( other_id )
                                        {
                                                zones->flag( func_vis_id, other_id );
                                        }
                                }
                        }

                        {
                                uint32_t          j;
                                BSPBoundingBox     bounds;
                                dface_t*        f = g_bspdata->dfaces + mod->firstface;

                                for ( j = 0; j < mod->numfaces; j++, f++ )
                                {
                                        Winding*        w = WindingFromFace( f );
                                        uint32_t          k;

                                        for ( k = 0; k < w->m_NumPoints; k++ )
                                        {
                                                bounds.add( w->m_Points[k] );
                                        }
                                        delete w;
                                }

                                zones->set( func_vis_id, bounds );

                                Log( "Adding zone %u : mins(%4.3f %4.3f %4.3f) maxs(%4.3f %4.3f %4.3f)\n", func_vis_id,
                                     bounds.m_Mins[0], bounds.m_Mins[1], bounds.m_Mins[2],
                                     bounds.m_Maxs[0], bounds.m_Maxs[1], bounds.m_Maxs[2] );
                        }
                }
        }

        return zones;
}
