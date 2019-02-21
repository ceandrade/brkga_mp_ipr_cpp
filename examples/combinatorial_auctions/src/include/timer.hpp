/******************************************************************************
 * timer.hpp: Interface/Implementation for system clock time class.
 *
 * Author: Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 *
 * (c) Copyright 2011 Institute of Computing, University of Campinas.
 *     All Rights Reserved.
 *
 *  Created on : 09/09/2011 by andrade
 *  Last update: 09/09/2011 by andrade
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>

#include <ctime>
#include <unistd.h>
#include <sys/time.h>

//  A timer object measures elapsed time.
//  It is recommended that implementations measure wall clock rather than CPU
//  time since the intended use is performance measurement on systems where
//  total elapsed time is more important than just process or CPU time.
//  Warnings: The maximum measurable elapsed time may well be only 596.5+ hours
//  due to implementation limitations.  The accuracy of timings depends on the
//  accuracy of timing information provided by the underlying platform, and
//  this varies a great deal from platform to platform.

/**
 * \brief System clock timer class.
 * \author Carlos Eduardo de Andrade <andrade@ic.unicamp.br>
 * \date 2011
 *
 * This class contains of a Timer based on system clock. It follows the same
 * interface of Boost::Timer, but it measures the real time. Resolution in
 * seconds.
 */
class Timer {
    public:
        /** \name Constructors and Destructor*/
        //@{
        /// Default constructor.
        Timer(): start_time() {
            gettimeofday(&start_time, 0);
        }
        //@}

        /** \name Member methods. */
        //@{
        /// Restarts the timer.
        void restart() {
            gettimeofday(&start_time, 0);
        }

        /// Return elapsed time in seconds.
        double elapsed() const {
            timeval end_time;
            gettimeofday(&end_time, 0);

            return (end_time.tv_sec + end_time.tv_usec/1000000.0) -
                   (start_time.tv_sec + start_time.tv_usec/1000000.0);
        }
        //@}

    protected:
        timeval start_time;
};
#endif // TIMER_HPP
