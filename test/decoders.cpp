/******************************************************************************
 * decoders.cpp: Simple decoders for testing.
 *
 * (c) Copyright 2019-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 *  Created on : Jun 27, 2019 by ceandrade
 *  Last update: Sep 15, 2023 by ceandrade
 *
 * This code is released under LICENSE.md.
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

#include "decoders.hpp"

#include <iostream>
using namespace std;

// Just sum the values.
double SumDecoder::decode(BRKGA::Chromosome& chromosome, bool /*non-used*/) {
    double total = 0.0;
    for(const auto &v : chromosome)
        total += v;

    return total;
}

// Counts if chromosome[i] < chromosome[i + 1].
double OrderDecoder::decode(BRKGA::Chromosome& chromosome, bool /*non-use*/) {
    // Just sum the values.
    double total = 0.0;
    double last = chromosome.front();
    for(const auto &v : chromosome) {
        if(last < v)
            total += 1.0;
        last = v;
    }

    return total;
}
