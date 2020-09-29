//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "../Batman5Files/bitarray.h"

namespace inet {

bitarray::bitarray() {}
bitarray::~bitarray() {}


/**
 * batadv_test_bit() - check if bit is set in the current window
 *
 * @seq_bits: pointer to the sequence number receive packet
 * @last_seqno: latest sequence number in seq_bits
 * @curr_seqno: sequence number to test for
 *
 * Return: true if the corresponding bit in the given seq_bits indicates true
 * and curr_seqno is within range of last_seqno. Otherwise returns false.
 */
bool Batman5Routing::batadv_test_bit(/*const unsigned long *seq_bits, */uint32_t last_seqno, uint32_t curr_seqno)
{
    int32_t diff;

    diff = last_seqno - curr_seqno;
    if (diff < 0 || diff >= BATADV_TQ_LOCAL_WINDOW_SIZE){
        return false;
    }
//    return test_bit(diff, seq_bits) != 0;
}


/**
 * batadv_bit_get_packet() - receive and process one packet within the sequence
 *  number window
 * @priv: the bat priv with all the soft interface information
 * @seq_bits: pointer to the sequence number receive packet
 * @seq_num_diff: difference between the current/received sequence number and
 *  the last sequence number
 * @set_mark: whether this packet should be marked in seq_bits
 *
 * Return: true if the window was moved (either new or very old),
 *  false if the window was not moved/shifted.
 */
bool Batman5Routing::batadv_bit_get_packet(/*unsigned long *seq_bits, */int32_t seq_num_diff, int set_mark)
{
    /* sequence number is slightly older. We already got a sequence number
     * higher than this one, so we just mark it. */
    if (seq_num_diff <= 0 && seq_num_diff > -BATADV_TQ_LOCAL_WINDOW_SIZE) {
        if (set_mark){
//            batadv_set_bit(seq_bits, -seq_num_diff);
        }
        return false;
    }

    /* sequence number is slightly newer, so we shift the window and
     * set the mark if required
     */
    if (seq_num_diff > 0 && seq_num_diff < BATADV_TQ_LOCAL_WINDOW_SIZE) {
//        batadv_bitmap_shift_left(seq_bits, seq_num_diff);

        if (set_mark){
//            batadv_set_bit(seq_bits, 0);
        }
        return true;
    }

    /* sequence number is much newer, probably missed a lot of packets */
    if (seq_num_diff >= BATADV_TQ_LOCAL_WINDOW_SIZE && seq_num_diff < BATADV_EXPECTED_SEQNO_RANGE){
        std::cout << "We missed a lot of packets (" << seq_num_diff - 1 << ") !" << std::endl;
//        bitmap_zero(seq_bits, BATADV_TQ_LOCAL_WINDOW_SIZE);
        if (set_mark){
//            batadv_set_bit(seq_bits, 0);
        }
        return true;
    }

    /* received a much older packet. The other host either restarted
     * or the old packet got delayed somewhere in the network. The
     * packet should be dropped without calling this function if the
     * seqno window is protected.
     *
     * seq_num_diff <= -BATADV_TQ_LOCAL_WINDOW_SIZE
     * or
     * seq_num_diff >= BATADV_EXPECTED_SEQNO_RANGE
     */
    std::cout << "Other host probably restarted!" << std::endl;

//    bitmap_zero(seq_bits, BATADV_TQ_LOCAL_WINDOW_SIZE);
    if (set_mark){
//        batadv_set_bit(seq_bits, 0);
    }

    return true;
}


/**
 * batadv_set_bit() - Turn corresponding bit on, so we can remember that we got
 *  the packet
 * @seq_bits: bitmap of the packet receive window
 * @n: relative sequence number of newly received packet
 */
void Batman5Routing::batadv_set_bit(/*unsigned long *seq_bits, */ int32_t n)
{
    /* if too old, just drop it */
    if (n < 0 || n >= BATADV_TQ_LOCAL_WINDOW_SIZE){
        return;
    }

//    set_bit(n, seq_bits); /* turn the position on */
}


/* shift the packet array by n places. */
void Batman5Routing::batadv_bitmap_shift_left(/*unsigned long *seq_bits, */ int32_t n)
{
    if (n <= 0 || n >= BATADV_TQ_LOCAL_WINDOW_SIZE){
        return;
    }

//    bitmap_shift_left(seq_bits, seq_bits, n, BATADV_TQ_LOCAL_WINDOW_SIZE);
}


} /* namespace inet */
