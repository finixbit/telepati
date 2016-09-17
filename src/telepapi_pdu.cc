/*
*
*	MIT License
*
*	Copyright (c) 2016 Intelburg
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*
*
*/

/*
 * This is a dummy PDU. It behaves very similarly to Tins::RawPDU.
 */
class DummyPDU : public PDU {
public:
    /* 
     * Unique protocol identifier. For user-defined PDUs, you **must**
     * use values greater or equal to PDU::USER_DEFINED_PDU;
     */
    static const PDU::PDUType pdu_flag;

    /*
     * Constructor from buffer. This constructor will be called while
     * sniffing packets, whenever a PDU of this type is found. 
     * 
     * The "data" parameter points to a buffer of length "sz". 
     */
    DummyPDU(const uint8_t* data, uint32_t sz) : buffer(data, data + sz) { }
    
    /*
     * Clones the PDU. This method is used when copying PDUs.
     */
    DummyPDU *clone() const { return new DummyPDU(*this); }
    
    /*
     * Retrieves the size of this PDU. 
     */
    uint32_t header_size() const { return buffer.size(); }
    
    /*
     * This method must return pdu_flag.
     */
    PDUType pdu_type() const { return pdu_flag; }
    
    /*
     * Serializes the PDU. The serialization output should be written
     * to the buffer pointed to by "data", which is of size "sz". The
     * "sz" parameter will be equal to the value returned by 
     * DummyPDU::header_size. 
     * 
     * The third parameter is a pointer to the parent PDU. You shouldn't
     * normally need to use this. 
     */
    void write_serialization(uint8_t *data, uint32_t sz, const PDU *parent) 
    { 
        std::copy(buffer.begin(), buffer.end(), data);
    }
    
    // This is just a getter to retrieve the buffer member.
    const std::vector<uint8_t> &get_buffer() const 
    {
        return buffer;
    }
private:
    std::vector<uint8_t> buffer;
};

// Let's assign some value to the pdu_flag.
const PDU::PDUType DummyPDU::pdu_flag = PDU::USER_DEFINED_PDU;