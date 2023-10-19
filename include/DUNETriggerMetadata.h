#ifndef DUNETRIGGERMETADATA_H
#define DUNETRIGGERMETADATA_H 1

#include "soa.h"
#include<vector>
namespace soa
{
    class DUNETriggerMetadata{
        public:
        DUNETriggerMetadata(){};

        ~DUNETriggerMetadata(){};

        struct FileParameters
        {
            //data and data structures...
            char app_name[512];
            char closing_time[512];
            char creation_time[512];
            char file_index[512];
            char layout_version[512];
            char op_environment[512];
            char rec_type[512];
            char rec_size[512];
            char run_number[512];
            char filelayout_version[128];
            char operational_environment[128];
            char record_type[128];
            char record_size[128];
            char run_number[128];
            filelayout_param FLay_Param;
            path_param_list Path_Param[5];
            source_id_geo_id_map geo_map[10];
        };
        //Two more sub data structures for layout parameters
        struct  filelayout_param
        {
            /* data */
            char digits_for_record_number[128];
            char digits_for_sequence_number[128];
            Path_Param_list path_param_list[10];
            char raw_data_group_name[128];
            char record_header_dataset_name[128];
            char record_name_prefix[128];
            char view_group_name[128];

        };

        struct path_param_list
        {
            /* data */
            char detector_group_name[128];
            char detector_group_type[128];
            char digits_for_element_number[128];
            char element_name_prefix[128];

        };
        
        struct source_id_geo_id_map{
            char geoids[128];
            char id[128];
            char subsys[1];
        };        
        
        
    };

} // namespace soa
#endif