#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void print_dict(py::dict dict) {
    /* Easily interact with Python types */
    for (auto item : dict)
        std::cout << "key=" << std::string(py::str(item.first)) << ", "
                  << "value=" << std::string(py::str(item.second)) << std::endl;
}


void generic(py::args args, py::kwargs kwargs) {
    /// .. do something with args
    if (kwargs) {
        
    }
        /// .. do something with kwargs
}


class Matrix {
public:
    Matrix(py::ssize_t rows, py::ssize_t cols) : m_rows(rows), m_cols(cols) {
        m_data = new float[rows*cols];
    }

    ~Matrix() {
        delete[] m_data;
    }

    Matrix &operator=(const Matrix &s) {
        delete[] m_data;
        m_rows = s.m_rows;
        m_cols = s.m_cols;
        m_data = new float[(size_t) (m_rows * m_cols)];
        memcpy(m_data, s.m_data, sizeof(float) * (size_t) (m_rows * m_cols));
        return *this;
    }

    Matrix &operator=(Matrix &&s) noexcept {
        if (&s != this) {
            delete[] m_data;
            m_rows = s.m_rows; m_cols = s.m_cols; m_data = s.m_data;
            s.m_rows = 0; s.m_cols = 0; s.m_data = nullptr;
        }
        return *this;
    }

    float operator()(py::ssize_t i, py::ssize_t j) const {
        return m_data[(size_t) (i*m_cols + j)];
    }

    float &operator()(py::ssize_t i, py::ssize_t j) {
        return m_data[(size_t) (i*m_cols + j)];
    }
    

    float *data() const { return m_data; }

    py::ssize_t rows() const { return m_rows; }
    py::ssize_t cols() const { return m_cols; }
private:
    py::ssize_t m_rows, m_cols;
    float *m_data;
};


PYBIND11_MODULE(variant_trans, m) {
    m.doc() = "pybind11 test variant_trans";
    m.def("print_dict",&print_dict);

    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def(py::init([](py::buffer const b) {
            py::buffer_info info = b.request();
            if (info.format != py::format_descriptor<float>::format() || info.ndim != 2)
                throw std::runtime_error("Incompatible buffer format!");

            auto v = new Matrix(info.shape[0], info.shape[1]);
            memcpy(v->data(), info.ptr, sizeof(float) * (size_t) (v->rows() * v->cols()));
            return v;
        }))

        .def("data", &Matrix::data, py::return_value_policy::reference)
        .def("rows", &Matrix::rows)
        .def("cols", &Matrix::cols)

              /// Bare bones interface
        .def("__getitem__",
             [](const Matrix &m, std::pair<py::ssize_t, py::ssize_t> i) {
                 if (i.first >= m.rows() || i.second >= m.cols())
                     throw py::index_error();
                 return m(i.first, i.second);
             })

        .def("__setitem__",
             [](Matrix &m, std::pair<py::ssize_t, py::ssize_t> i, float v) {
                 if (i.first >= m.rows() || i.second >= m.cols())
                     throw py::index_error();
                 m(i.first, i.second) = v;
             })

        .def("__repr__",
            [](const Matrix &m) {
                std::cout<<"<variant_trans.Matrix rows:"<<m.rows()<<","
                <<"cols: "<<m.cols()<<"\n";

                float *data = m.data();
                int rows = m.rows();
                int cols = m.cols();
                
                std::cout<<"[";
                for(int r=0;r<rows;r++) {
                    std::cout<<"[";
                    for(int c=0;c<cols;c++) {
                        std::cout<<data[r*cols + c];
                        if(c != (cols -1)) {
                            std::cout<<" ";
                        }
                    }
                    std::cout<<"]";
                    if(r != (rows-1)) {
                        std::cout<<std::endl;
                    }
                    
                }
                std::cout<<"]"<<std::endl;

                return "";
            })

        .def_buffer([](Matrix &m) -> py::buffer_info {
            return py::buffer_info(
                m.data(),                               /* Pointer to buffer */
                sizeof(float),                          /* Size of one scalar */
                py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
                2,                                      /* Number of dimensions */
                { m.rows(), m.cols() },                 /* Buffer dimensions */
                { sizeof(float) * m.cols(),             /* Strides (in bytes) for each index */
                sizeof(float) }
            );
        });

}