/* +---------------------------------------------------------------------------+
	 |                     Mobile Robot Programming Toolkit (MRPT)               |
	 |                          http://www.mrpt.org/                             |
	 |                                                                           |
	 | Copyright (c) 2005-2017, Individual contributors, see AUTHORS file        |
	 | See: http://www.mrpt.org/Authors - All rights reserved.                   |
	 | Released under BSD License. See details in http://www.mrpt.org/License    |
	 +---------------------------------------------------------------------------+ */

#ifndef CFIXEDINTERVALSNRD_H
#define CFIXEDINTERVALSNRD_H

#include <mrpt/obs/CObservationOdometry.h>
#include <mrpt/obs/CSensoryFrame.h>
#include <mrpt/obs/CActionCollection.h>
#include <mrpt/obs/CRawlog.h>
#include <mrpt/utils/types_simple.h>
#include <mrpt/system/threads.h>

#include <mrpt/graphslam/interfaces/CIncrementalNodeRegistrationDecider.h>

namespace mrpt { namespace graphslam { namespace deciders {

/**\brief Fixed Intervals Odometry-based Node Registration
 *
 * ## Description
 *
 * Determine whether to insert a new pose in the graph given the distance and
 * angle thresholds. When the odometry readings indicate that any of the
 * thresholds has been surpassed, with regards to the previous registered
 * pose, a new node is added in the graph.
 *
 * Current decider is a minimal, simple implementation of the
 * CNodeRegistrationDecider interface which can be used for 2D datasets.
 * Decider *does not guarantee* thread safety when accessing the GRAPH_T
 * resource. This is handled by the CGraphSlamEngine instance.
 *
 * ### Specifications
 *
 * - Map type: 2D, 3D
 * - MRPT rawlog format: #1, #2
 * - Graph Type: CPosePDFGaussianInf
 * - Observations Used: CObservationOdometry, CActionRobotMovement2D, CActionRobotMovement3D
 * - Node Registration Strategy: Fixed Odometry Intervals
 *
 * \ingroup mrpt_graphslam_grp
 */
template<class GRAPH_T=typename mrpt::graphs::CNetworkOfPoses2DInf>
class CFixedIntervalsNRD:
	public virtual mrpt::graphslam::deciders::CIncrementalNodeRegistrationDecider<GRAPH_T>
{
	public:
		// Public functions
		//////////////////////////////////////////////////////////////

		/**\brief Handy typedefs */
		/**\{*/
		/**\brief Node Registration Decider */
		typedef mrpt::graphslam::deciders::CNodeRegistrationDecider<GRAPH_T> node_reg;
		typedef mrpt::graphslam::deciders::CIncrementalNodeRegistrationDecider<GRAPH_T> incr_reg;
		typedef incr_reg parent_t;

		/**\brief type of graph constraints */
		typedef typename GRAPH_T::constraint_t constraint_t;
		/**\brief type of underlying poses (2D/3D). */
		typedef typename GRAPH_T::constraint_t::type_value pose_t;
		typedef typename GRAPH_T::global_pose_t global_pose_t;

		typedef mrpt::math::CMatrixFixedNumeric<double,
						constraint_t::state_length,
						constraint_t::state_length> inf_mat_t;
		/**\}*/

		/**\brief Class constructor */
		CFixedIntervalsNRD();
		/**\brief Class destructor */
		~CFixedIntervalsNRD();


		void loadParams(const std::string& source_fname);
		void printParams() const;
		void getDescriptiveReport(std::string* report_str) const;

		/**\brief Method makes use of the CActionCollection/CObservation to update the
		 * odometry estimation from the last inserted pose
		 *
		 * \return True upon successful node registration in the graph
		 */
		bool updateState(mrpt::obs::CActionCollectionPtr action,
				mrpt::obs::CSensoryFramePtr observations,
				mrpt::obs::CObservationPtr observation );

	protected:
		// protected functions
		//////////////////////////////////////////////////////////////

		// protected members
		//////////////////////////////////////////////////////////////

		/**\brief pose_t estimation using only odometry information. Handy for
		 * observation-only rawlogs.
		 */
		pose_t m_curr_odometry_only_pose;
		/**\brief pose_t estimation using only odometry information. Handy for
		 * observation-only rawlogs.
		 */
		pose_t m_last_odometry_only_pose;
		/**\brief Keep track of whether we are reading from an observation-only
		 * rawlog file or from an action-observation rawlog
		 */
		bool m_observation_only_rawlog;

};

} } } // end of namespaces

#include "CFixedIntervalsNRD_impl.h"
#endif /* end of include guard: CFIXEDINTERVALSNRD_H */
