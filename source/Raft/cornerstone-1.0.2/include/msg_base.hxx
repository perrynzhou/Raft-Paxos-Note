/**
*
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements.  The ASF licenses
* this file to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef _MSG_BASE_HXX_
#define _MSG_BASE_HXX_
namespace cornerstone {

    class msg_base {
    public:
        msg_base(ulong term, msg_type type, int src, int dst)
            : term_(term), type_(type), src_(src), dst_(dst) {}

        virtual ~msg_base() {}

        ulong get_term() const {
            return this->term_;
        }

        msg_type get_type() const {
            return this->type_;
        }

        int32 get_src() const {
            return this->src_;
        }

        int32 get_dst() const {
            return this->dst_;
        }

    __nocopy__(msg_base)

    private:
        ulong term_;
        msg_type type_;
        int32 src_;
        int32 dst_;
    };
}
#endif //_MSG_BASE_HXX_