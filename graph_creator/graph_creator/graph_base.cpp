#include "graph_base.h"


graph_base::graph_base()
    {
        state_ = {0, 0, 0, 0, 0, 1, 0, 0};
        //state_ = {0, 0, 0, 0, 0, 0, 0};
        oper_ = {0, 0, 0, 0, 0, 0};
    }

bool graph_base::Is_Value_Read() const
{
    return state_.is_input;
}

void graph_base::Set_Read(bool st)
{
    state_.is_input = st;
}

bool graph_base::Is_Work_With_Node_Edge() const
{
    return state_.is_work_with_node_egde;
}

void graph_base::Set_Work_With_Node_Edge(bool st)
{
    state_.is_work_with_node_egde = st;
}

bool graph_base::What_Is_Object() const
{
    return state_.is_ne;
}

void graph_base::Set_Object(bool st)
{
    state_.is_ne = st;
}

bool graph_base::Is_Edge_Drawing() const
{
     return state_.is_draft;
}

void graph_base::Set_Edge_Draw(bool st)
{
    state_.is_draft = st;
}


bool graph_base::Is_Edge_Moves() const
{
    return state_.is_edge_moves;
}

void graph_base::Set_Edge_Moves(bool st)
{
    state_.is_edge_moves = st;
}

bool graph_base::Is_Node_Moves() const
{
    return state_.is_moves;
}

void graph_base::Set_Node_Moves(bool st)
{
    state_.is_moves = st;
}

bool graph_base::Is_Orient() const
{
    return state_.is_orient_graph_;
}

void graph_base::Set_Orient(bool st)
{
    state_.is_orient_graph_ =st;
}

bool graph_base::Is_Draw_Two_Edge() const
{
    return state_.is_work_with_node_egde;
}

void graph_base::Set_Draw_Two_Edge(bool st)
{
    state_.is_work_with_node_egde = st;
}

bool graph_base::Is_Write_Inp() const
{
    return oper_.is_w_input_;
}

void graph_base::Set_Write_Inp(bool st)
{
    oper_.is_w_input_ = st;
}

bool graph_base::Is_Read_Inp() const
{
    return oper_.is_r_input_;
}

void graph_base::Set_Read_Inp(bool st)
{
    oper_.is_r_input_ = st;
}

bool graph_base::Is_Hight_Light() const
{
    return oper_.is_high_light_;
}

void graph_base::Set_Hight_Light(bool st)
{
    oper_.is_high_light_ = st;
}

bool graph_base::Is_Bbm() const
{
    return oper_.is_bbm_;
}

void graph_base::Set_Bbm(bool st)
{
    oper_.is_bbm_ = st;
}

bool graph_base::Is_Poten() const
{
    return oper_.is_poten_;
}

void graph_base::Set_Poten(bool st)
{
    oper_.is_poten_ = st;
}

bool graph_base::Is_Info() const
{
    return oper_.is_info_;
}

void graph_base::Set_Info(bool st)
{
    oper_.is_info_ = st;
}

