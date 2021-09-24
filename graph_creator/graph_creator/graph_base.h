#ifndef GRAPH_BASE_H_INCLUDED
#define GRAPH_BASE_H_INCLUDED

#include <cstddef>
#include <vector>
#include <cstring>

class graph_base
{
    struct stat
    {
        bool is_input : 1;          //если сейчас процессввод
        bool is_work_with_node_egde : 1;
        bool is_ne : 1;     //режим ввода данныхдля вершины/дуги (0 - вершина, 1 - дуга)
        bool is_draft : 1;          //если рисуется дуга/грань
        bool is_moves : 1;          //если перемещаем вершину
        bool is_orient_graph_ : 1;  //если граф ориентирован
        bool is_draft_two_edg : 1;
        bool is_edge_moves : 1;
    } state_;

    struct
    {
        bool is_w_input_ : 1;      //если ввод имена файла для чтения/записи
        bool is_r_input_ : 1;      //если ввод имена файла для чтения/записи
        bool is_high_light_ : 1;    //если граф выделен
        bool is_bbm_ : 1;
        bool is_poten_ : 1;
        bool is_info_ : 1;
    } oper_;

public:

    typedef float type_t;
    typedef std::vector<std::vector<type_t>> matrix;
    int counter_ = 0;

    graph_base();

    bool Is_Value_Read() const;
    void Set_Read(bool st);

    bool Is_Work_With_Node_Edge() const;
    void Set_Work_With_Node_Edge(bool st);

    bool What_Is_Object() const;
    void Set_Object(bool st);

    bool Is_Edge_Drawing() const;
    void Set_Edge_Draw(bool st);

    bool Is_Edge_Moves() const;
    void Set_Edge_Moves(bool st);

    bool Is_Node_Moves() const;
    void Set_Node_Moves(bool st);

    bool Is_Orient() const;
    void Set_Orient(bool st);

    bool Is_Draw_Two_Edge() const;
    void Set_Draw_Two_Edge(bool st);

    bool Is_Write_Inp() const;
    void Set_Write_Inp(bool st);

    bool Is_Read_Inp() const;
    void Set_Read_Inp(bool st);

    bool Is_Hight_Light() const;
    void Set_Hight_Light(bool st);

    bool Is_Bbm() const;
    void Set_Bbm(bool st);

    bool Is_Poten() const;
    void Set_Poten(bool st);

    bool Is_Info() const;;
    void Set_Info(bool st);

    char const * get_name() const;
};

#endif // GRAPH_BASE_H_INCLUDED
