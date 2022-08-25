#pragma once

// добавьте неоходимые include-директивы сюда
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
	// напишите код класса тут
	// Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType — совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType> 
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            // Реализуйте конструктор самостоятельно
            node_ = node; //QUESTIONS!
        }
    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            // Реализуйте конструктор самостоятельно
            node_ = other.node_; //QUESTIONS!
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            if ((node_ == nullptr && rhs.node_ == nullptr) || (node_ == rhs.node_)) {
                return true;
            }
            return false;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            return !(*this == rhs); // We need * in "this"?
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            if ((this->node_ == nullptr && rhs.node_ == nullptr) || (node_ == rhs.node_)) {
                return true;
            }
            return false;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            return !(*this == rhs); // We need * in "this"?
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            auto next = node_->next_node; // Correct? Isn't it?
            node_ = next;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            //assert(false);
            //Реализуйте оператор самостоятельно
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            return node_->value; //Right?
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            //assert(false);
            // Реализуйте оператор самостоятельно
            return &node_->value; //Mmm?
        }

    private:
        Node* node_ = nullptr;
    };

public:
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() {
        while (head_.next_node != nullptr) {//1)как осуществляется удаление узла?
            Node* node = head_.next_node->next_node;//2)ответ: через дополнительный адрес, который указывает на нод header_, что указыает на нод следующий после
            delete head_.next_node; //1)ответ: удаление осуществляется конкретно узла(нода)
            head_.next_node = node; //2)как корректно присвоить адрес следующего узла?
        }
        size_ = 0;
    }

    ~SingleLinkedList() {
        Clear();
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept { 
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        if (size_ == 0) {
            return true;
        }
        return false;
    }

    using value_type = Type; 
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        //assert(false);
        // Реализуйте самостоятельно
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        //assert(false);
        // Реализуйте самостоятельно
        auto last = head_.next_node;
        for (int i= 0; i < size_; ++i) {
            auto temp = last->next_node;
            last = temp;
        }
        return Iterator(last);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        //assert(false);
        // Реализуйте самостоятельно
        return ConstIterator(head_.next_node);;
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        //assert(false);
        // Реализуйте самостоятельно
        auto last = head_.next_node;
        for (int i = 0; i < size_; ++i) {
            auto temp = last->next_node;
            last = temp;
        }
        return ConstIterator(last);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        //assert(false);
        // Реализуйте самостоятельно
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        //assert(false);
        // Реализуйте самостоятельно
        auto last = head_.next_node;
        for (int i = 0; i < size_; ++i) {
            auto temp = last->next_node;
            last = temp;
        }
        return ConstIterator(last);
    } 

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) { 
        MakeSingleLinkedList(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList& other) {
        MakeSingleLinkedList(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        // Реализуйте присваивание самостоятельно
        //assert(size_ == 0 && head_.next_node == nullptr);

        SingleLinkedList tmp(rhs);

        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(tmp);
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        // Реализуйте обмен содержимого списков самостоятельно
        size_t temp_size = size_;
        size_ = other.size_;
        other.size_ = temp_size;

        Node temp_head = head_;
        head_.next_node = other.head_.next_node;
        other.head_.next_node = temp_head.next_node;
    }




    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        // Реализуйте самостоятельно
        return Iterator(&head_);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        // Реализуйте самостоятельно
        auto link = const_cast<Node*>(&head_);
        return ConstIterator(link);
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        // Реализуйте самостоятельно
        auto link = const_cast<Node*>(&head_);
        return ConstIterator(link);
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        // Реализуйте метод самостоятельно
        if (pos == before_begin()) {
            head_.next_node = new Node(value, head_.next_node);
            ++size_;
            return Iterator(head_.next_node);
        }
        pos.node_->next_node = new Node(value, pos.node_->next_node); //
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    void PopFront() noexcept {
        // Реализуйте метод самостоятельно
        EraseAfter(before_begin());
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        // Реализуйте метод самостоятельно
        if (pos == before_begin()) {
            auto temp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete temp;
            --size_;
            return Iterator(head_.next_node);
        }
        
        auto temp = pos.node_->next_node;
        auto next = pos.node_->next_node->next_node;
        pos.node_->next_node = next;
        delete temp;
        --size_;
        return Iterator(next);
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_ = {};
    size_t size_ = 0;

    template<typename Iterator>
    void MakeSingleLinkedList(Iterator begin, Iterator end) {
        SingleLinkedList tmp;

        Node** tmp_ptr = &tmp.head_.next_node;
        for (auto it = begin; it != end; ++it) { //например так
            *tmp_ptr = new Node(*it, nullptr);
            tmp_ptr = &((*tmp_ptr)->next_node);
        }

        swap(tmp);
        size_ = std::distance(begin, end);
    }
};

// внешние функции разместите здесь
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    // Реализуйте обмен самостоятельно
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Реализуйте сравнение самостоятельно
    if (std::equal(lhs.begin(), lhs.end(), rhs.begin())) {
        return true;
    }
    return false;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Реализуйте сравнение самостоятельно
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Реализуйте сравнение самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Реализуйте сравнение самостоятельно
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Реализуйте сравнение самостоятельно
    return !(lhs < rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    // Реализуйте сравнение самостоятельно
    return (lhs > rhs) || (lhs == rhs);
}
